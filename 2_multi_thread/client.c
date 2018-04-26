#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

typedef struct MySocketInfo{
    int socketCon;
    unsigned long ipaddr;
    unsigned short port;
}_MySocketInfo;

void *fun_thrReceiveHandler(void *socketCon);
int checkThrIsKill(pthread_t thr);

int main(int argc, char const *argv[])
{
    printf("开始socket\n");
    /* 创建TCP连接的Socket套接字 */
    int socketCon = socket(AF_INET, SOCK_STREAM, 0);
    if(socketCon < 0){
        printf("创建TCP连接套接字失败\n");
        exit(-1);
    }
    /* 填充客户端端口地址信息，以便下面使用此地址和端口监听 */
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1"); /* 这里地址使用全0，即所有 */
    server_addr.sin_port=htons(2001);
    /* 连接服务器 */
    int res_con = connect(socketCon,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr));
    if(res_con != 0){
        printf("连接失败\n");
        exit(-1);
    }
    printf("连接成功,连接结果为：%d\n",res_con);
    //开启新的实时接受数据线程
    pthread_t thrReceive;
    pthread_create(&thrReceive,NULL,fun_thrReceiveHandler,&socketCon);

    /* 实时发送数据 */

    if(argc!=2){
        fprintf(stderr, "usage %s content\n", argv[0]);
        exit(1);
        }


    char userStr[30] ;
    // 可以录入用户操作选项，并进行相应操作
    strncpy(userStr,argv[1],sizeof(argv[1]));
    printf("userStr is %s \n",userStr);
    /*
    while(1){
        //检测接受服务器数据线程是否被杀死
        //printf("please input :\n");
        //scanf("%s",userStr);
        if(strcmp(userStr,"q") == 0){
            printf("用户选择退出！\n");
            break;
        }
        // 发送消息
        //int sendMsg_len = send(socketCon, userStr, 30, 0);
        int sendMsg_len = write(socketCon,userStr,30);
        if(sendMsg_len > 0){
            printf("发送成功,服务端套接字句柄:%d\n",socketCon);
        }else{
            printf("发送失败\n");
        }

        //if(checkThrIsKill(thrReceive) == 1){
            //printf("接受服务器数据的线程已被关闭，退出程序\n");
            //break;
        //}
        
        printf("waiting for this 100 seconds\n");
        sleep(100);
        
        //break;
    }
    */
    // 关闭套接字
    printf("waiting for this 20 seconds\n");
    sleep(20);
    close(socketCon);
    return 0;
}

void *fun_thrReceiveHandler(void *socketCon){
    /*
    while(1){
        char buffer[100];
        int _socketCon = *((int *)socketCon);
        //int buffer_length = recv(_socketCon,buffer,30,0);
        int buffer_length = read(_socketCon,buffer,100);
        if(buffer_length == 0){
            printf("服务器端异常关闭\n");
            exit(-1);
        }else if(buffer_length < 0){
            printf("接受客户端数据失败\n");
            break;
        }
        buffer[buffer_length] = '\0';
        printf("服务器说：%s\n",buffer);
    }

    */
    
    char buffer[100];
    int _socketCon = *((int *)socketCon);
    //int buffer_length = recv(_socketCon,buffer,30,0);
    int buffer_length = read(_socketCon,buffer,100);
    if(buffer_length == 0){
        printf("服务器端异常关闭\n");
        exit(-1);
    }else if(buffer_length < 0){
        printf("接受客户端数据失败\n");
        //break;
    }
    buffer[buffer_length] = '\0';
    printf("服务器说：%s\n",buffer);
    

    printf("退出接受服务器数据线程\n");
    return NULL;
}

int checkThrIsKill(pthread_t thr){
    int res = 1;
    int res_kill = pthread_kill(thr,0);
    if(res_kill == 0){
        res = 0;
    }
    return res;
}
