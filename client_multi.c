#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

 typedef struct MySocketInfo{
 	int socketCon;
 	unsigned long ipaddr;
 	unsigned short port;
 }_MySocketInfo;


 void *fun_thrReceiveHandler(void *socketCon);
 int checkThrIsKill(pthread_t thr);

 int checkThrIsKill(pthread_t thr){
 	int res=1;
 	int res_kill = pthread_kill(thr,0);
 	if(res_kill==0){
 		res=0;
 	}
 	return res;
 }



 void *fun_thrReceiveHandler(void *socketCon){
 	while(1){
 		char buffer[30];
 		int _socketCon=*((int*)socketCon);
 		int buffer_length=read(_socketCon,buffer,30);
 		if(buffer_length==0){
 			printf("server clsoed unexcepted\n");
 			exit(-1);
 		}else if(buffer_length<0){
 			printf("receive the client data fail\n");
 			break;
 		}
 		buffer[buffer_length]='\0';
 		printf("server give out data: %s\n", buffer);
 	}

 	printf("the srever receive data thread quit\n");
 	return NULL;
 }


int send_one_times(int port_num){
	int socketCon=socket(AF_INET,SOCK_STREAM,0);
	if(socketCon<0){
		printf("create the socket fail\n");
		exit(-1);
	}
	//fill the client port ip info,and use the address to listen
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(struct sockaddr_in));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	server_addr.sin_port=htons(port_num);

	//connect to the server 
	int res_con=connect(socketCon,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr));
	if(res_con!=0){
		printf("connect fail\n");
		exit(-1);
	}

	printf("connect success, the res is : %d\n",res_con);
	pthread_t thread_receive;
	pthread_create(&thread_receive,NULL,fun_thrReceiveHandler,&socketCon);

	int count_send=0;
	


	while(count_send<1){

		char userStr[30];
		//itoa(port_num,userStr,10);
		sprintf(userStr, "%d", port_num);
		//scanf("%s",userStr);
		printf("userStr is : %s\n", userStr);
        if(strcmp(userStr,"q") == 0){
            printf("用户选择退出！\n");
            break;
        }
        // 发送消息
        //int sendMsg_len = send(socketCon, userStr, 30, 0);
		int sendMsg_len=write(socketCon,userStr,30);

		if(sendMsg_len>0){
			printf("send message success,the handler is %d:\n",socketCon);
		}else{
			printf("send fail\n");
		}

		count_send++;
	}

	close(socketCon);
	return 1;
}



int main(int argc, char const *argv[])
{
	send_one_times(2000);
	return 0;
}