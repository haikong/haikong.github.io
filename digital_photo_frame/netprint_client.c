#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
/*
*socket
*connect
*send/recv
*/

#define SERVER_PORT 8888

/*
 *./netprint_client debuglevel=<0-9>
 *./netprint_client setclient
 *./netprint_client stdout = 0 | 1
 *./netprint_client show
*/

int main(int argc,char** argv)
{
    int iSocketClient;
    ssize_t iSendLen;
    ssize_t iRecvLen;
    socklen_t iAddrLen;
    char strRecvBuf[1000];
    struct sockaddr_in tSocketServerAddr;

    if(argc != 3){
        printf("Usage : %s <serverip> dbglevelh=<0-9>.\n",argv[0]);
        printf("Usage : %s <serverip> setclient\n",argv[0]);
        printf("Usage : %s <serverip> stdout= 0 | 1\n",argv[0]);
        printf("Usage : %s <serverip> show\n",argv[0]);
        return -1;
    }

    iSocketClient = socket(AF_INET,SOCK_DGRAM,0);
    if(-1 == iSocketClient){
        printf("socket error\n");
        return -1;
    }

    tSocketServerAddr.sin_family        = AF_INET;
    tSocketServerAddr.sin_port          = htons(SERVER_PORT);
    //tSocketServerAddr.sin_addr.s_addr   = INADDR_ANY;
    if( 0 == inet_aton(argv[1],&tSocketServerAddr.sin_addr))
    {
        printf("Invalid serverip.\n");
        close(iSocketClient);
        return -1;
    }
    memset(tSocketServerAddr.sin_zero ,0 ,8);

    if(0 == strcmp(argv[2],"show"))
    {
        iSendLen = sendto(iSocketClient,"setclient",strlen("setclient"),0,
                            (const struct sockaddr*)&tSocketServerAddr,sizeof(struct sockaddr));
        /*循环从网络读数据打印出来*/
        while(1)
        {

            iRecvLen =  recvfrom(iSocketClient,strRecvBuf,999,0,
                                 (struct sockaddr *)&tSocketServerAddr,&iAddrLen);
            if(iRecvLen > 0)
            {
                strRecvBuf[iRecvLen] = '\0';
                printf("Get Message from %s:%s\n",inet_ntoa(tSocketServerAddr.sin_addr),strRecvBuf);
            }
        }

    }
    else {
        iSendLen = sendto(iSocketClient,argv[2],strlen(argv[2]),0,
                            (const struct sockaddr*)&tSocketServerAddr,sizeof(struct sockaddr));
    }
    close(iSocketClient);
    return 0;
}

