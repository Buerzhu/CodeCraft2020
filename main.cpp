//#include<bits/stdc++.h>
#include<iostream>
#include<time.h>
#include<string>
#include<string.h>
#include<algorithm>
#include<vector>
#include<unordered_map>
#include<utility>
#include<atomic>
#include <sys/time.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <pthread.h>

using namespace std;
#define INF 0x7fffffff
#define MAX_ID 300000000 //id��ֵ����ӳ�������С�ڸ���ֵ��idʹ������ӳ�䣬���ڸ�id��ʹ��unorder_mapӳ��
#define MAX_G 201326592 //1.5G����һ��һά����G���洢���нڵ���ӽڵ���Ϣ��Ԥ�ȷ���1.5G�ռ�;��һ��һά����GR���洢���нڵ�ĸ��ڵ���Ϣ��Ԥ�ȷ���1.5G�ռ�
#define MAX_EDGE 2000001 //����������Ϊ200w
#define MAXN 2000001 //�������ڵ���Ϊ200w
#define MAX_LEN 7
#define MIN_LEN 3

#define ansLen3 33*4000000 //level3,Ϊÿ���߳��г���Ϊ3�Ļ�Ԥ�ȷ���Ŀռ䣬��λΪ�ֽ�
#define ansLen4 44*4000000 //level4��Ϊÿ���߳��г���Ϊ4�Ļ�Ԥ�ȷ���Ŀռ䣬��λΪ�ֽ�
#define ansLen5 55*4000000 //level5��Ϊÿ���߳��г���Ϊ5�Ļ�Ԥ�ȷ���Ŀռ䣬��λΪ�ֽ�
#define ansLen6 66*4000000 //level6��Ϊÿ���߳��г���Ϊ6�Ļ�Ԥ�ȷ���Ŀռ䣬��λΪ�ֽ�
#define ansLen7 77*4000000 //level7��Ϊÿ���߳��г���Ϊ7�Ļ�Ԥ�ȷ���Ŀռ䣬��λΪ�ֽ�

#define likely(x) __builtin_expect(!!(x), 1) 
#define unlikely(x) __builtin_expect(!!(x), 0)

#define THREAD_NUM 6//�̳߳��߳���
#define BLOCKS 100//�̵߳����������ʹ����id��Ŀ
//#define PRINT


pair<int,int> G[MAX_G]; //�洢ÿ���ڵ�ĳ����ӽڵ��ת��ֵ
pair<int,int> GR[MAX_G]; //�洢ÿ���ڵ����ȸ��ڵ��ת��ֵ

char idComma[MAXN][15]; //�洢�ڵ�id���ַ�����ʽ����������ַ�
char idLF[MAXN][15]; //�洢�ڵ�id���ַ�����ʽ����������ַ�
char idCharLen[MAXN]; //�洢ÿ���ڵ�ת��Ϊ�ַ���֮����ַ�������
char* addrOffset[MAX_LEN-MIN_LEN+1][MAXN]; //ÿ���ڵ㻷�洢���ڴ�ƫ�Ƶ�ַ����һά��ʾ�����ȣ�����Ϊ3�Ļ���Ӧ����Ϊ0�����ڶ�ά��ʾӳ���Ľڵ�id��ӳ���Ľڵ�id��1��ʼ��
char* addrStart[MAX_LEN-MIN_LEN+1][MAXN]; //ÿ���ڵ㻷�洢���ڴ���ʼ��ַ����һά��ʾ�����ȣ�����Ϊ3�Ļ���Ӧ����Ϊ0�����ڶ�ά��ʾӳ���Ľڵ�id��ӳ���Ľڵ�id��1��ʼ��

int GInd[MAXN][2];  //ÿ���ڵ���pair<int,int> G[MAX_G]�洢�ĳ����ӽڵ��ռ�õĿռ�ĵ�ַ��[0]��ʾռ�ÿռ���׵�ַ��[1]��ʾռ�ÿռ��β��ַ
int GRInd[MAXN][2];  //ÿ���ڵ���pair<int,int> GR[MAX_G]�洢����ȸ��ڵ��ռ�õĿռ�ĵ�ַ��[0]��ʾռ�ÿռ���׵�ַ��[1]��ʾռ�ÿռ��β��ַ
int idInDeg[MAXN];//ÿ���ڵ�������
int idOutDeg[MAXN];//ÿ���ڵ�ĳ�����
int nodePairs[MAX_EDGE*3];  //�洢����δӳ���id��Ϣ��ת����Ϣ������test_data.txt��ȡ��ÿһ�е���Ϣ
int temp[MAX_EDGE*2]; //���ڴ洢nodePairs��id��Ϣ
bool idUnique[MAX_ID]; //��־λ����־ĳid֮ǰ�Ƿ񱻷��ʹ�
int idMapArr[MAX_ID]; //hash���飬�ø���������С��MAX_ID��id����idӳ�䣬����MAX_ID��ʹ��unordered_map����ӳ��

int minMoney=1000;
int blockSize; //���нڵ㻮�ֺ���������
int edgeNum; //����
int nodeNum; //�ڵ���
int idFreeStart=1;//��ǰδ���䵽�̴߳����id����С�ڵ�

std::atomic_flag flag(ATOMIC_FLAG_INIT);//ԭ�ӱ�����ʼ�������øñ�����ԭ����������������

struct timeval startTime;
struct timeval endTime;


struct P3
{   //P3�ṹ��
    int id[2]; //id��Ϣ
    int cc[2]; //ת����Ϣ
};

//�̳߳�ʼ��
struct Thread
{
    pthread_t tid;

	//�洢��Ӧ���ȵĻ�
    char ans3[ansLen3];
    char ans4[ansLen4];
    char ans5[ansLen5];
    char ans6[ansLen6];
    char ans7[ansLen7];

    int8_t nodeLen[MAXN];

    int pMapLen[MAXN];//P1
    int pMapMoney[MAXN];//��·���ĵ�һ��money

    P3 pMap3[15000][200];
    int pMap3Len[MAXN][3];//pMap3Len[i][0]��i�Ķ�Ӧ��㣬pMap3Len[i][1]��k�ĸ���,pMap3Len[i][2]��i��pMap��λ��
    int iP3[MAXN];
    int resCnt;

};


void setTime() {
    gettimeofday(&startTime, NULL);
}

int getTime() {
    gettimeofday(&endTime, NULL);
    return int(1000 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000);
}

//��ȡtest_data.txt��ÿһ����Ϣ���洢��nodePairs��
void readTxtMmap(string& inputFile){
    int fd = open(inputFile.c_str(), O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    char *buffer = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(buffer==nullptr || buffer==(void*)-1){
      close(fd);
      exit(-1);
    }
    close(fd);

    int val=0;
    edgeNum=0;
    bool flag=true;
    while(*buffer)
    {
        if(*buffer<'0') // || *buffer>'9'
        {
            if(flag)
            {

                nodePairs[edgeNum]=val;
                ++edgeNum;
                val=0;
                flag=false;
            }
        }
        else
        {
            val = val*10 + (*buffer - '0');
            //val = (val<<3) + (val<<1) + (*buffer - '0');
            flag=true;
        }
        *buffer++;
    }
    munmap(buffer, sb.st_size);
}


Thread* threadPool;

void buildGraph(string &inputFile){

    setTime();
    readTxtMmap(inputFile);
    threadPool=new Thread[THREAD_NUM];

    int sumID=0;
	//��һ��������Ϊ�˼���temp�е��ظ�id�����Ӷ�����sort�����ʱ��
    for(int i=0,j=0;i<edgeNum;i+=3)
    {
        if(nodePairs[i]<MAX_ID){ 
            if(idUnique[nodePairs[i]]==false){
                temp[j]=nodePairs[i];
                ++j;
                idUnique[nodePairs[i]]=true;
                ++sumID;
            }
        }else{
            temp[j]=nodePairs[i];
            ++j;
            ++sumID;
        }

        if(nodePairs[i+1]<MAX_ID){ //���id����MAX_ID����ʹ������ӳ��
            if(idUnique[nodePairs[i+1]]==false){
                temp[j]=nodePairs[i+1];
                ++j;
                idUnique[nodePairs[i+1]]=true;
                ++sumID;
            }
        }else{
            temp[j]=nodePairs[i+1];
            ++j;
            ++sumID;
        }
    }
    sort(temp,temp+sumID);


    //��ȡ���ظ�id�ڵ�
    for(int i=0;i<sumID;++i)
    {
        if(temp[i]!=temp[nodeNum]){
            ++nodeNum;
            temp[nodeNum]=temp[i];
        }
    }
    ++nodeNum;


    
    unordered_map<int,int> idMap;
    char str[]="0123456789";
    int num,k;
	//����idӳ�䲢��idת��Ϊ�ַ�����ʽ��������滷�Ĵ洢
    for(int i=0;i<nodeNum;i++)
    {
        if(temp[i]<MAX_ID){
            idMapArr[temp[i]]=i+1;
        }else{
            idMap[temp[i]]=i+1;
        }
        num=temp[i];
        k=0;
        while(num>=0)
        {
            if(num<10)
            {
                idComma[i+1][k++]=str[num];
                idComma[i+1][k++]=',';
                idComma[i+1][k]='\0';
                break;
            }
            idComma[i+1][k++]=str[num%10];
            num/=10;
        }
        for(int j=0;j<=(k-2)/2;++j)
        {
            swap(idComma[i+1][j],idComma[i+1][k-2-j]);
        }
        idCharLen[i+1]=k;
    }
    for(int i=1;i<=nodeNum;++i)
    {
        memcpy(idLF[i],idComma[i],idCharLen[i]);
        idLF[i][idCharLen[i]-1]='\n';
    }

	//nodePairs�洢ӳ���Ľڵ�id��Ϣ
    for(int i=0;i<edgeNum;i+=3)
    {
        nodePairs[i]=nodePairs[i]<MAX_ID?idMapArr[nodePairs[i]]:idMap[nodePairs[i]];
        nodePairs[i+1]=nodePairs[i+1]<MAX_ID?idMapArr[nodePairs[i+1]]:idMap[nodePairs[i+1]];
    }

    int u,v,c;
	//ͳ����Ⱥͳ�����Ϣ
    for(int i=0;i<edgeNum;i+=3)
    {
        u=nodePairs[i];
        v=nodePairs[i+1];
        ++GRInd[v][1];
        ++GInd[u][1];
    }
	//��¼ÿ���ڵ�ĸ��ڵ���ӽڵ���GR��G�ж�Ӧ��λ����Ϣ
    for(int i=2;i<=nodeNum;++i)
    {
        GRInd[i][0]=GRInd[i-1][1];
        GRInd[i][1]+=GRInd[i-1][1];

        GInd[i][0]=GInd[i-1][1];
        GInd[i][1]+=GInd[i-1][1];
    }

    pair<int,int> tmpPair;
	//��G�д洢ÿ���ڵ���ӽڵ���Ϣ
    for(int i=0;i<edgeNum;i+=3)
    {
        u=nodePairs[i];
        v=nodePairs[i+1];
        c=nodePairs[i+2];
        tmpPair=make_pair(v,c);
        memcpy(G+GInd[u][0]+idOutDeg[u],&tmpPair,8);
        ++idOutDeg[u];  //����ڵ�ĳ�����
    }

#ifdef PRINT
    printf("EdgeNum: %d\n",edgeNum/3);
    printf("NodeNum: %d\n",nodeNum);
    printf("AveDegs: %lf\n",double(edgeNum)/nodeNum/3.0);
    cout<<"Build graph time: "<<getTime()<<endl;
#endif

}


void rmNode(){
    setTime();
	//��ÿ���ڵ���ӽڵ����id��С��������
    for(int i=1;i<=nodeNum;++i){
        if(idOutDeg[i]>1){
            sort(G+GInd[i][0],G+GInd[i][1]);
        }
    }

    int u,v,c;
	//��GR�д洢ÿ���ڵ�ĸ��ڵ���Ϣ
    for(int i=1;i<=nodeNum;++i){
        for(int j=GInd[i][0];j<GInd[i][1];++j){
            u=i;
            v=G[j].first;
            c=G[j].second;
            GR[GRInd[v][0]+idInDeg[v]]=make_pair(u,c);
            ++idInDeg[v];
        }
    }
#ifdef PRINT
    cout<<"Topo sort time: "<<getTime()<<endl;
#endif
}



static bool cmp(P3 a,P3 b);
//�һ����滷
void doDFS(int index)
{
    setTime();
    Thread& target=threadPool[index];

    int head,i,j,k,l,m,i_it,j_it,k_it,l_it,m_it,ii,iii,z;
    int k0,k1,k2,k3,kk0,kk1,kk2,kk3,plen;
    int idInDeghead,idInDegkk3,idInDegkk2,idInDegkk1,idOutDegi,idOutDegj,idOutDegk,idOutDegl,pMap3Lenmm;
    int cc1,cc2,cc3,ccj,cck,ccL,ccm,ccFirst,ccLast,minccj,maxccj;
    int pAddr;
    int pInd,pInd2;
    int path0,path1,path2,path3,path4,path5,path6;
    bool vis[MAXN];  //�ڵ��Ƿ񱻷���

    int localResCnt=0,iP3Len=0;
    int idStart,idEnd;
    int64_t left=5,right=3;
    int64_t ccjLeft,ccjRight;

    char *addrOffset0=target.ans3,*addrOffset1=target.ans4,*addrOffset2=target.ans5,*addrOffset3=target.ans6,*addrOffset4=target.ans7;
    char *addrBlock0,*addrBlock1,*addrBlock2,*addrBlock3,*addrBlock4;

    if(THREAD_NUM==1){
        blockSize=nodeNum;
    }else{
        blockSize=BLOCKS;
    }

	//�߳̿�ʼ����������ȡ�����������id
    while(flag.test_and_set());//���������
    idStart=idFreeStart;
    idFreeStart+=blockSize;//idFreeStartֻ���ڼ��������
    idEnd=idFreeStart;
    flag.clear();//����

	//��ʼ�����ֳ��Ȼ��Ĵ洢��ַ
    addrBlock0=addrOffset0;
    addrBlock1=addrOffset1;
    addrBlock2=addrOffset2;
    addrBlock3=addrOffset3;
    addrBlock4=addrOffset4;

    for(int mm=0;mm<nodeNum;++mm)
    {
		//�����ǰ������id���������
        if(idStart==idEnd){
            //��¼ÿ�������ַ
            addrBlock0=addrOffset0;
            addrBlock1=addrOffset1;
            addrBlock2=addrOffset2;
            addrBlock3=addrOffset3;
            addrBlock4=addrOffset4;

			//��ʼ��һ��������
            while(flag.test_and_set());//���������
            idStart=idFreeStart;
            idFreeStart+=blockSize;//idFreeStartֻ���ڼ��������
            idEnd=idFreeStart;
            flag.clear();//����
        }

        i=idStart;
        if(i>nodeNum)
            break;

        if(idOutDeg[i]>0)
        {
            head=i;

            for(int i_it=0;i_it<iP3Len;++i_it)
            {
                target.nodeLen[target.iP3[i_it]]=0;//���ó���Ϊ4
            }

            iP3Len=0;
            idInDeghead=GRInd[head][1];
            for(k3=GRInd[head][0];k3<idInDeghead;++k3) //����P2
            {
                kk3=GR[k3].first;
                cc3=GR[k3].second;
                if(kk3<=head)  // || cc3>left*maxccj || right*cc3<minccj
                    continue;

                target.pMapLen[kk3]=head;//���й���P1
                target.pMapMoney[kk3]=cc3;

                idInDegkk3=GRInd[kk3][1];
                for(k2=GRInd[kk3][0];k2<idInDegkk3;++k2)
                {
                    kk2=GR[k2].first;
                    cc2=GR[k2].second;
                    if(kk2<=head || cc3*left<cc2 || cc3>right*cc2)
                        continue;

                    idInDegkk2=GRInd[kk2][1];
                    for(k1=GRInd[kk2][0];k1<idInDegkk2;++k1)
                    {
                        kk1=GR[k1].first;
                        cc1=GR[k1].second;
                        if(kk1<=head || kk1==kk3 || cc2*left<cc1 || cc2>right*cc1)
                            continue;

                        if(target.pMap3Len[kk1][0]!=head){
                            target.pMap3Len[kk1][0]=head;
                            target.pMap3Len[kk1][1]=0;
                            target.pMap3Len[kk1][2]=iP3Len;
                            target.iP3[iP3Len]=kk1;//��kk2,ͬ�±����ظ�
                            target.nodeLen[kk1]=3;
                            ++iP3Len;

                        }
                        pInd=target.pMap3Len[kk1][1];
                        pInd2=target.pMap3Len[kk1][2];
                        target.pMap3[pInd2][pInd].id[0]=kk2;
                        target.pMap3[pInd2][pInd].id[1]=kk3;
                        target.pMap3[pInd2][pInd].cc[0]=cc1;
                        target.pMap3[pInd2][pInd].cc[1]=cc3;
                        ++target.pMap3Len[kk1][1];
                    }
                }
            }

            for(int pi=0;pi<iP3Len;++pi)
            {
                pAddr=target.pMap3Len[target.iP3[pi]][1];
                if(pAddr>1){
                    sort(target.pMap3[pi],target.pMap3[pi]+pAddr,cmp);//��P3����
                }
            }


            //��ʼѰ��
            vis[i]=true;
            path0=i;
            idOutDegi=GInd[i][1];
            for(i_it=GInd[i][0];i_it<idOutDegi;++i_it)//level2
            {
                ccj=G[i_it].second;
                if(G[i_it].first <= head)
                    continue;
                j=G[i_it].first;
                ccjLeft=ccj*left;
                ccjRight=ccj*right;

                vis[j]=true;
                path1=j;

                if(target.pMap3Len[j][0]==head)
                {
                    pAddr=target.pMap3Len[j][2];
                    pMap3Lenmm=target.pMap3Len[j][1];
                    for(z=0;z<pMap3Lenmm;++z)
                    {
                        ccFirst=target.pMap3[pAddr][z].cc[0];
                        ccLast=target.pMap3[pAddr][z].cc[1];
                        if(ccFirst*left<ccj || ccFirst>ccjRight || ccjLeft<ccLast || ccj>right*ccLast)
                            continue;

                        path2=target.pMap3[pAddr][z].id[0];
                        path3=target.pMap3[pAddr][z].id[1];
                        memcpy((void *) addrOffset1,idComma[path0], idCharLen[path0]);
                        addrOffset1 += idCharLen[path0];
                        memcpy((void *) addrOffset1,idComma[path1], idCharLen[path1]);
                        addrOffset1 += idCharLen[path1];
                        memcpy((void *) addrOffset1,idComma[path2], idCharLen[path2]);
                        addrOffset1 += idCharLen[path2];
                        memcpy((void *) addrOffset1,idLF[path3], idCharLen[path3]);
                        addrOffset1 += idCharLen[path3];
                        ++localResCnt;
                    }
                }

                idOutDegj=GInd[j][1];
                for(j_it=GInd[j][0];j_it<idOutDegj;++j_it)//level3
                {
                    cck=G[j_it].second;
                    if(G[j_it].first <= head || cck*left<ccj || cck>ccjRight)
                        continue;
                    k=G[j_it].first;

                    vis[k]=true;
                    path2=k;

                    if(target.pMapLen[k]==head)
                    {
                        ccLast=target.pMapMoney[k];
                        if(ccLast*left>=cck && ccLast<=right*cck && ccjLeft>=ccLast && ccj<=right*ccLast)
                        {
                            memcpy((void *) addrOffset0,idComma[path0], idCharLen[path0]);
                            addrOffset0 += idCharLen[path0];
                            memcpy((void *) addrOffset0,idComma[path1], idCharLen[path1]);
                            addrOffset0 += idCharLen[path1];
                            memcpy((void *) addrOffset0,idLF[path2], idCharLen[path2]);
                            addrOffset0 += idCharLen[path2];
                            ++localResCnt;
                        }
                    }

                    if(target.pMap3Len[k][0]==head)//level6
                    {
                        pAddr=target.pMap3Len[k][2];
                        pMap3Lenmm=target.pMap3Len[k][1];
                        for(z=0;z<pMap3Lenmm;++z)
                        {
                            if(vis[target.pMap3[pAddr][z].id[0]] || vis[target.pMap3[pAddr][z].id[1]])
                                continue;

                            ccFirst=target.pMap3[pAddr][z].cc[0];
                            ccLast=target.pMap3[pAddr][z].cc[1];
                            if(ccFirst*left<cck || ccFirst>right*cck || ccjLeft<ccLast || ccj>right*ccLast)
                                continue;

                            path3=target.pMap3[pAddr][z].id[0];
                            path4=target.pMap3[pAddr][z].id[1];
                            memcpy((void *) addrOffset2,idComma[path0], idCharLen[path0]);
                            addrOffset2 += idCharLen[path0];
                            memcpy((void *) addrOffset2,idComma[path1], idCharLen[path1]);
                            addrOffset2 += idCharLen[path1];
                            memcpy((void *) addrOffset2,idComma[path2], idCharLen[path2]);
                            addrOffset2 += idCharLen[path2];
                            memcpy((void *) addrOffset2,idComma[path3], idCharLen[path3]);
                            addrOffset2 += idCharLen[path3];
                            memcpy((void *) addrOffset2,idLF[path4], idCharLen[path4]);
                            addrOffset2 += idCharLen[path4];
                            ++localResCnt;
                        }
                    }

                    idOutDegk=GInd[k][1];
                    for(k_it=GInd[k][0];k_it<idOutDegk;++k_it)//level4
                    {
                        ccL=G[k_it].second;
                        l=G[k_it].first;
                        if(G[k_it].first <= head || ccL*left<cck || ccL>right*cck)////////////////////
                            continue;

                        if(vis[l]==false)
                        {
                            vis[l]=true;
                            path3=l;
                            if(target.pMap3Len[l][0]==head)//level7
                            {
                                pAddr=target.pMap3Len[l][2];
                                pMap3Lenmm=target.pMap3Len[l][1];
                                for(z=0;z<pMap3Lenmm;++z)
                                {
                                    if(vis[target.pMap3[pAddr][z].id[0]] || vis[target.pMap3[pAddr][z].id[1]])
                                        continue;

                                    ccFirst=target.pMap3[pAddr][z].cc[0];
                                    ccLast=target.pMap3[pAddr][z].cc[1];
                                    if(ccFirst*left<ccL || ccFirst>right*ccL || ccjLeft<ccLast || ccj>right*ccLast)
                                        continue;

                                    path4=target.pMap3[pAddr][z].id[0];
                                    path5=target.pMap3[pAddr][z].id[1];

                                    memcpy((void *) addrOffset3,idComma[path0], idCharLen[path0]);
                                    addrOffset3 += idCharLen[path0];
                                    memcpy((void *) addrOffset3,idComma[path1], idCharLen[path1]);
                                    addrOffset3 += idCharLen[path1];
                                    memcpy((void *) addrOffset3,idComma[path2], idCharLen[path2]);
                                    addrOffset3 += idCharLen[path2];
                                    memcpy((void *) addrOffset3,idComma[path3], idCharLen[path3]);
                                    addrOffset3 += idCharLen[path3];
                                    memcpy((void *) addrOffset3,idComma[path4], idCharLen[path4]);
                                    addrOffset3 += idCharLen[path4];
                                    memcpy((void *) addrOffset3,idLF[path5], idCharLen[path5]);
                                    addrOffset3 += idCharLen[path5];
                                    ++localResCnt;
                                }
                            }

                            idOutDegl=GInd[l][1];
                            for(l_it=GInd[l][0];l_it<idOutDegl;++l_it)
                            {
                                ccm=G[l_it].second;
                                m=G[l_it].first;
                                if(target.nodeLen[m]!=3 || G[l_it].first <= head || ccm*left<ccL || ccm>right*ccL)
                                    continue;

                                if(vis[m]==false)//level5
                                {
                                    vis[m]=true;
                                    path4=m;

                                    pAddr=target.pMap3Len[m][2];
                                    pMap3Lenmm=target.pMap3Len[m][1];
                                    for(z=0;z<pMap3Lenmm;++z)
                                    {
                                        if(vis[target.pMap3[pAddr][z].id[0]] || vis[target.pMap3[pAddr][z].id[1]])
                                            continue;

                                        ccFirst=target.pMap3[pAddr][z].cc[0];
                                        ccLast=target.pMap3[pAddr][z].cc[1];
                                        if(ccFirst*left<ccm || ccFirst>right*ccm || ccjLeft<ccLast || ccj>right*ccLast)
                                            continue;

                                        path5=target.pMap3[pAddr][z].id[0];
                                        path6=target.pMap3[pAddr][z].id[1];

                                        memcpy((void *) addrOffset4,idComma[path0], idCharLen[path0]);
                                        addrOffset4 += idCharLen[path0];
                                        memcpy((void *) addrOffset4,idComma[path1], idCharLen[path1]);
                                        addrOffset4 += idCharLen[path1];
                                        memcpy((void *) addrOffset4,idComma[path2], idCharLen[path2]);
                                        addrOffset4 += idCharLen[path2];
                                        memcpy((void *) addrOffset4,idComma[path3], idCharLen[path3]);
                                        addrOffset4 += idCharLen[path3];
                                        memcpy((void *) addrOffset4,idComma[path4], idCharLen[path4]);
                                        addrOffset4 += idCharLen[path4];
                                        memcpy((void *) addrOffset4,idComma[path5], idCharLen[path5]);
                                        addrOffset4 += idCharLen[path5];
                                        memcpy((void *) addrOffset4,idLF[path6], idCharLen[path6]);
                                        addrOffset4 += idCharLen[path6];
                                        ++localResCnt;

                                    }

                                    vis[m]=false;
                                }
                            }
                            vis[l]=false;
                        }
                    }
                    vis[k]=false;
                }
                vis[j]=false;
            }
            //vis[i]=false;
        }

        if(idStart==idEnd-1 || i==nodeNum){
            addrStart[0][i]=addrBlock0;
            addrStart[1][i]=addrBlock1;
            addrStart[2][i]=addrBlock2;
            addrStart[3][i]=addrBlock3;
            addrStart[4][i]=addrBlock4;

            addrOffset[0][i]=addrOffset0;
            addrOffset[1][i]=addrOffset1;
            addrOffset[2][i]=addrOffset2;
            addrOffset[3][i]=addrOffset3;
            addrOffset[4][i]=addrOffset4;
        }

        idStart++;
    }

    target.resCnt = localResCnt;

#ifdef PRINT
    cout<<"Index "<<index<<" DoDFS time: "<<getTime()<<endl;
#endif
}


/*
void saveTxtMmap(string &outputFile){
    auto t=clock();
    char temp[100];

    resCnt=0;
    for(int x=0;x<5;++x){
        resCnt += ansCntAll[x];
    }

    int idx=sprintf(temp,"%d\n",resCnt);
    temp[idx]='\0';
    int totalSize=0;    //��¼·���Ĵ�С
    for(int i=MIN_LEN;i<=MAX_LEN;++i) {
        totalSize += (addrOffset[i-MIN_LEN]-addrStart[i-MIN_LEN]);
    }
    totalSize+=idx;
    int fd = open(outputFile.c_str(),O_CREAT|O_RDWR|O_TRUNC,S_IRUSR|S_IWUSR);//�������ö�дȨ��,��ֻ��дȨ��,�����SIGSEGV�ź�
                                                            //mmap�����ļ�ӳ��ʱ�����ȶ�ȡ�ļ�`
    int len=lseek(fd,totalSize-1,SEEK_CUR);  //�ӵ�ǰλ�ÿ�ʼƫ�� totalSize-1���ַ�
    int n=write(fd,"",1);
    char *addr = (char *)mmap(NULL, totalSize, PROT_WRITE,MAP_SHARED,fd,0);
    int ansOffset=0;
    memcpy(addr,temp,idx);
    ansOffset+=idx;
    for(int i=MIN_LEN;i<=MAX_LEN;++i) {
        memcpy(addr+ansOffset,ans[i-MIN_LEN],addrOffset[i-MIN_LEN]-addrStart[i-MIN_LEN]);
        ansOffset+=(addrOffset[i-MIN_LEN]-addrStart[i-MIN_LEN]);
    }
    close(fd);
    munmap((void *)addr, totalSize);
#ifdef PRINT
    printf("Total Loops: %d\n",resCnt);
    cout<<"Mmap save time: "<<clock()-t<<endl;
#endif
}*/

//дresult.txt
void saveTxt(const string &outputFile){
    setTime();
    FILE *fp = fopen(outputFile.c_str(), "wb");
    int resCnt=0;
    for(int i=0;i<THREAD_NUM;i++)
    {
        resCnt+=threadPool[i].resCnt;
    }
    char temp[100];
    int idx=sprintf(temp,"%d\n",resCnt);
    temp[idx]='\0';
    fwrite(temp,idx,sizeof(char),fp);

    int j;
    for(int i=MIN_LEN;i<=MAX_LEN;++i) {
        for(j=blockSize+1;j<nodeNum;j+=blockSize){
            fwrite(addrStart[i-MIN_LEN][j-1],addrOffset[i-MIN_LEN][j-1]-addrStart[i-MIN_LEN][j-1],1,fp);
        }
        if(j-blockSize!=nodeNum+1)
            fwrite(addrStart[i-MIN_LEN][nodeNum],addrOffset[i-MIN_LEN][nodeNum]-addrStart[i-MIN_LEN][nodeNum],1,fp);
    }
    fclose(fp);
#ifdef PRINT
    printf("Total Loops: %d\n",resCnt);
    cout<<"Save time: "<<getTime()<<endl;
#endif
}


static void* work(void* arg);

//��ʼ�������߳�
void createThreadPool()
{

    for(int i=0;i<THREAD_NUM;i++)
    {
        int* index=new int(i);//�߳����̳߳��е����
        if(pthread_create(&threadPool[i].tid,NULL,work,index)!=0)//����̴߳���ʧ��
        {
            printf("create thread failed\n");
            exit(0);
        }
        else
        {
#ifdef PRINT
            printf("create thread: %ld\n",threadPool[i].tid);
#endif
        }
    }

    for(int i=THREAD_NUM-1;i>=0;i--)//�ȴ��������߳���ֹ�������߳���Դ
    {
        pthread_join(threadPool[i].tid,NULL);
    }
}


//���̵߳Ĺ�������
void* work(void* arg)
{
    int* p=(int*)arg;
    int index=*p;//index�����߳����̳߳��е����
    doDFS(index);
    return NULL;
}
//P3��sort���������
bool cmp(P3 a,P3 b)
{
    return (a.id[0]!=b.id[0])?(a.id[0]<b.id[0]):(a.id[1]<b.id[1]);
}


int main()
{
    //string inputFile = "test_data_2000w.txt";  //"/data/test_data.txt"
    //string outputFile = "output.txt";  //"/projects/student/result.txt"

    string inputFile = "/data/test_data.txt"; //"/data/test_data.txt"
    string outputFile = "/projects/student/result.txt"; //"/projects/student/result.txt"

    buildGraph(inputFile);
    rmNode();
    createThreadPool();
    saveTxt(outputFile);

    return 0;
}

