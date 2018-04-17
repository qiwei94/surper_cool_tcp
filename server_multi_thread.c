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


void * fun_therad_accept_handler(void *socketlisten){
	while(1){
		int sockaddr_in_size=sizeof(struct sockaddr_in);
		struct sockaddr_in client_addr;
		int _socketListen=*((int *)socketlisten);
		int socketCon=accept(_socketListen,(struct sockaddr*)(&client_addr),(socklen_t *)(&sockaddr_in_size));
		if(socketCon<0){
			printf("connect fail\n");
		}else{
			printf("connect sucess ip: %s:%d\n",inet_ntoa(client_addr.sin_addr),client_addr.sin_port);
		}
		printf("the connect socket is :%d\n",socketCon);

		//open a new communication thread , and communicate with the client
		_my_socket_info socketinfo;
		socketinfo.socketCon=socketCon;
		socketinfo.ipaddr=inet_ntoa(client_addr.sin_addr);
		socketinfo.port=client_addr.sin_port;
		Arr_Thread_Receive_Client[conClientCount]=socketinfo;
		conClientCount++;
		printf("now get  client conneted\n");

		pthread_t thread_receive=0;
		pthread_create(&thread_receive,NULL,fun_thread_receive_handler,&socketinfo);
		Arr_Thread_Receive_Client[thread_receive_client_count]=thread_receive;
		thread_receive_client_count++;

		//make the thread sleep for 0.5s
		sleep(0.5);
	}
	char *s="exit the thread safely";
	pthread_exit(s);
}


void *fun_thread_receive_handler(void *socketinfo){
	char buffer[30];
	int buffer_length
	_my_socket_info _socketInfo=*((_my_socket_info*)socketinfo);
	while(1){
		//buffer to be zero
		bzero(&buffer,sizeof(buffer));

		buffer_length=read(_socketInfo,socketCon,buffer,30);
		if(buffer_length==0){
			printf("%s:%d client closed\n", _socketInfo.ipaddr,_socketInfo.port);
			conClientCount--;
			break;
		}else if (buffer_length<0)
		{
			printf("receive client data fail \n");
			break;
		}
		buffer[buffer_length]='\0';
		printf("%s:%d saied that: %s\n", _socketInfo.ipaddr,_socketInfo.port,buffer);
		sleep(0.2);
	}
	printf("receive data thread stop\n");
	return NULL;
}


int check_thread_is_killed(pthread_t thr){
	int res=1;
	int res_kill = pthread_kill(thr,0);
	if(res_kill == 0){
		res=0;
	}
	return res;
}










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
		sleep(0,5);
	}

	printf("wait for thread quit\n");
	char* message;
	pthread_join(thread_accept,(void *)&message);
	printf("%s\n",message);

	return 0;
}




