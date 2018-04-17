#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>  
#include <sys/socket.h>  
#include <string.h>  
#include <signal.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <pthread.h>  



//on the server
void *fun_thread_receive_handler(void *socketinfo);
void *fun_therad_accept_handler(void *socketlisten);


//1: yes,killed 0: no not be killed
int check_thread_is_killed(pthread_t thr);

typedef struct my_socket_info
{
	int socketCon;
	char *ipaddr;
	uint16_t port;
}_my_socket_info;


//client vector
struct my_socket_info arr_socket[10];
int conClientCount=0;

//the client thread list
pthread_t Arr_Thread_Receive_Client[10];
int thread_receive_client_count=0;

int main(int argc, char const *argv[])
{
	printf("socket begin\n");

	//init the socket 
	int socketlisten=socket(AF_INET,SOCK_STREAM,0);
	if(socketlisten<0){
		printf("socket create fail\n");
		exit(-1);
	}else{
		printf("create sucess!\n");
	}


	//fill the socket with ip and port
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(struct sockaddr_in));
	server_addr.sin_family=AF_INET;
	//use inaddr any to match every ip
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(2000);

	if(bind(socketlisten,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)!=0)){
		perror("bind ip port fail");
		exit(-1);
	}else{
		printf("ip and port bind sucess\n");
	}

	//listen to the ip and port
	if(listen(socketlisten,10)!=0){
		printf("listen fail\n");
		exit(-1);
	}else{
		printf("listen success\n");
	}

	//multi_thread accept socket
	pthread_t thread_accept;
	pthread_create(&thread_accept,NULL,fun_therad_accept_handler,&socketlisten);

	//send the data all the time
	while(1){
		//judge how many thread is still alive
		int i;
		for(int i=0;i<thread_receive_client_count;i++){
			if(check_thread_is_killed(Arr_Thread_Receive_Client[i])==1){
				printf("there is a thread been killed\n");
				thread_receive_client_count--;
			}
		}
		printf("now, the accept thread num is :%d\n",thread_receive_client_count);

		//get user operation ,and do it
		char user_str[30]={'0'};
		scanf("%s",user_str);
		if(strcmp(user_str,"q")==0){
			printf("user get out \n");
			break;
		}

		//send message
		if(conClientCount<=0){
			printf("no clinet connect\n");
		}else{
			int i;
			for(int i=0;i<conClientCount;i++){
				int sendMsg_len=write(arr_socket[i].socketCon,user_str,30);
				if(sendMsg_len>0){
					printf("send to %s:%s success\n",arr_socket[i].ipaddr,arr_socket[i].port);
				}else{
					printf("send to %s:%s fail\n",arr_socket[i].ipaddr,arr_socket[i].port);
				}
			}
		}

	}


	return 0;
}
