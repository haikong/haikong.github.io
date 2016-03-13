#include <config.h>
#include <debug_manager.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

#define SERVER_PORT     8888
#define PRINT_BUF_SIZE (16*1024)

static int NetDbgInit(void);
static int NetDbgExit(void);
static int NetDebugPrint(char* strData);
static int isFull(void);
static int isEmpty(void);
static int  PutData(char cVal);
static int  GetData(char *pcVal);


static T_DebugOpr g_tNetDbgOpr = {
	.name           = "netprint",
	.isCanUse       = 1,
	.DebugInit      = NetDbgInit,
	.DebugExit      = NetDbgExit,
	.DebugPrint     = NetDebugPrint,
};

static int g_iSocketServer;
static struct sockaddr_in g_tSocketServerAddr;
static struct sockaddr_in g_tSocketClientAddr;
static int iHaveConnected = 0;
static char* g_pcNetPrintBuf;
static int g_iReadPos = 0;
static int g_iWritePos = 0;
static pthread_t g_iSendThreadID;
static pthread_t g_iRecvThreadID;
/*��������*/
static pthread_mutex_t g_tNetMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_tNetConVar = PTHREAD_COND_INITIALIZER;


static void* NetSendThreadFunction(void* pVoid)
{
    char strTmpBuf[512];
    char cVal;
    int i,iSendLen;
    while((iHaveConnected) && (!isEmpty())){
        /*ƽʱ����״̬*/
        pthread_mutex_lock(&g_tNetMutex);
        pthread_cond_wait(&g_tNetConVar,&g_tNetMutex);
        pthread_mutex_unlock(&g_tNetMutex);
        /*���Ѻ�ѻ��λ�����������sendto�������ʹ�ӡ��Ϣ���ͻ���*/
        while(1)
        {
            i = 0;
            /*�ӻ��λ�����ȡ���������512�ֽ�*/
            while( (i < 512) && (0 == GetData(&cVal)) )
            {
                strTmpBuf[i++] = cVal;
            }


            iSendLen = sendto(g_iSocketServer,strTmpBuf,i,0,
                                (const struct sockaddr*)&g_tSocketClientAddr,sizeof(struct sockaddr));

        }
    }
    return NULL;
}

static void* NetRecvThreadFunction(void* pVoid)
{
    int iRecvLen;
    socklen_t iAddrLen;
    char ucRecvBuff[1000] = "\0";
    struct sockaddr_in tSocketClientAddr;

    iAddrLen = sizeof(struct sockaddr);
    while(1)
    {
       iRecvLen =  recvfrom(g_iSocketServer,ucRecvBuff,999,0,
                                (struct sockaddr *)&tSocketClientAddr,&iAddrLen);
       /*��������
       *dbglevel = 0,1 ....:�޸Ĵ�ӡ����
       *stdout = 0 ....�ر�stdout��ӡ
       *stdout = 1 ....��stdout��ӡ
       *netprint = 0 ....�ر�netprint��ӡ
       *netprint = 1 ....��netprint��ӡ
       *setclient       ���ý��ܴ�ӡ��Ϣ�Ŀͻ���
       */
       if(iRecvLen > 0){
            ucRecvBuff[iRecvLen] = '\0';
            if(0 == strcmp(ucRecvBuff,"setclient")){
                g_tSocketClientAddr = tSocketClientAddr;
                iHaveConnected = 1;
            }
            else if(0 == strncmp(ucRecvBuff,"dbglevel=",9))
            {
                SetDbgLevel(ucRecvBuff);
            }
            else{
                SetDbgChanel(ucRecvBuff);
            }

       }
    }
    return NULL;

}

static int NetDbgInit(void)
{
    /*socket ��ʼ��*/
    int iRet;
    g_iSocketServer =  socket(AF_INET,SOCK_DGRAM,0);
    if(-1 == g_iSocketServer){
        printf("socket error\n");
        return -1;
    }

    g_tSocketServerAddr.sin_family        = AF_INET;
    g_tSocketServerAddr.sin_port          = htons(SERVER_PORT);
    g_tSocketServerAddr.sin_addr.s_addr   = INADDR_ANY;
    memset(g_tSocketServerAddr.sin_zero ,0 ,8);
    iRet = bind(g_iSocketServer,(struct sockaddr *)&g_tSocketServerAddr,sizeof(struct sockaddr));
    if(-1 == iRet){
        perror("bind error");
        return -1;
    }

    g_pcNetPrintBuf = (char*)malloc(PRINT_BUF_SIZE);
    if(!g_pcNetPrintBuf)
    {
        close(g_iSocketServer);
        printf("malloc error\n");
        return -1;
    }


    /*���������߳�:�������ʹ�ӡ��Ϣ���ͻ���*/
    pthread_create(&g_iSendThreadID,NULL,NetSendThreadFunction,NULL);

    /*���������߳�:�������ܿ���������޸Ĵ�ӡ����*/

    pthread_create(&g_iRecvThreadID,NULL,NetRecvThreadFunction,NULL);
    return 0;
}

static int NetDbgExit(void)
{
    /*socket �ر�*/
    free(g_pcNetPrintBuf);
    close(g_iSocketServer);
    return 0;
}

static int isFull(void)
{
    return (((g_iWritePos + 1) % PRINT_BUF_SIZE) == g_iReadPos);
}

static int isEmpty(void)
{
    return (g_iReadPos == g_iWritePos);
}


static int  PutData(char cVal)
{
    if(isFull())
        return -1;
    else
    {
        g_pcNetPrintBuf[g_iWritePos] = cVal;
        g_iWritePos = (g_iWritePos + 1) % PRINT_BUF_SIZE;
    }
    return 0;
}

static int  GetData(char *pcVal)
{
    if(isEmpty())
        return -1;
    else
    {
        *pcVal = g_pcNetPrintBuf[g_iReadPos];
        g_iReadPos = (g_iReadPos + 1) % PRINT_BUF_SIZE;
    }
    return 0;
}


static int NetDebugPrint(char* strData)
{
    /*�����ݷ��뻷�λ�����*/
    int i;
    for(i = 0;i < strlen(strData);i++)
    {
        if(0 != PutData(strData[i]))
            break;
    }
    /*����пͻ������ӣ��Ͱ�����ͨ�����緢�͸��ͻ���*/
    /*���ѷ����߳�*/
    pthread_mutex_lock(&g_tNetMutex);
    pthread_cond_signal(&g_tNetConVar);
    pthread_mutex_unlock(&g_tNetMutex);
    return i;
}

int NetPrintInit(void)
{
	return RegisterDebugOpr(&g_tNetDbgOpr);
}

