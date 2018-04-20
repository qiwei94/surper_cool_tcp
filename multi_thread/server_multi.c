#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>  
#include <sys/socket.h>  
#include <string.h>  
#include <signal.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <pthread.h>  

#define total_sock_num 1

//on the server
// the both of the two funcuion is used to receive and accept??
void *fun_thread_receive_handler(void *socketinfo);
void *fun_therad_accept_handler(void *socketlisten);


//1: yes,killed 0: no not be killed
int check_thread_is_killed(pthread_t thr);


// a socket struct with socket : ip addr , port 
typedef struct my_socket_info
{
	int socketCon;
	char *ipaddr;
	uint16_t port;
}_my_socket_info;


//client vector, a vector combined with 10 my_socket_info struct 
struct my_socket_info arr_socket[10000];
// the counter of how many client
int conClientCount=0;

//the client thread list ,the vector used to store the receive thread
pthread_t Arr_Thread_Receive_Client[10000];
//the receive thread count 
int thread_receive_client_count=0;


//this is to say , many socket and many thread used together to finish the job

int check_thread_is_killed(pthread_t thr){
	int res=1;
	int res_kill = pthread_kill(thr,0);
	if(res_kill == 0){
		res=0;
	}
	return res;
}

void * fun_therad_accept_handler(void *socketlisten){
	while(1){//while 1 means it do the accept all the time
		//get the struct sockaddr_in size
		int sockaddr_in_size=sizeof(struct sockaddr_in);
		//a struct to store client addr info 
		struct sockaddr_in client_addr;
		
		//what means the socket listen???
		int _socketListen=*((int *)socketlisten);
		

		/*
		accept funciton:
		int accept(int sockfd, void *addr, int *addrlen);
		sockfd:the file descripter of socket
		addr 是个指 向局部的数据结构 sockaddr_in 的指针
		这是要求接入的信息所要去的地 方（你可以测定那个地址在那个端口呼叫你）。
		the length is the sizeof(struct sockaddr_in)
		so the _socketListen is the sockdet fd
		*/
		int socketCon=accept(_socketListen,(struct sockaddr*)(&client_addr),(socklen_t *)(&sockaddr_in_size));
		if(socketCon<0){
			printf("connect fail\n");
			//pass;
		}else{
			printf("connect sucess ip: %s:%d\n",inet_ntoa(client_addr.sin_addr),client_addr.sin_port);
		}
		printf("the connect socket is :%d\n",socketCon);

		//open a new communication thread , and communicate with the client
		//construct a new struct socketinfo,and init it
		_my_socket_info socketinfo;
		socketinfo.socketCon=socketCon;
		socketinfo.ipaddr=inet_ntoa(client_addr.sin_addr);
		socketinfo.port=client_addr.sin_port;

		// put it into the receive client vector
		
		arr_socket[conClientCount]=socketinfo;
		conClientCount++;
		printf("now get %d client conneted\n",conClientCount);
		//create a new thread and use it to run receive data handler
		pthread_t thread_receive=0;
		pthread_create(&thread_receive,NULL,fun_thread_receive_handler,&socketinfo);
		
		
		//add the thread to the vector
		Arr_Thread_Receive_Client[thread_receive_client_count]=thread_receive;
		thread_receive_client_count++;
		printf("now the receive thread num is :%d\n", thread_receive_client_count);
		//make the thread sleep for 0.5s
		
		sleep(1);
	}

	char *s="exit the thread safely";
	pthread_exit(s);
}





void *fun_thread_receive_handler(void *socketinfo){
	char buffer[30];
	int buffer_length;

	//use the input to init the socket_info
	_my_socket_info _socketInfo=*((_my_socket_info*)socketinfo);
	while(1){
		//buffer to be zero
		bzero(&buffer,sizeof(buffer));


		//read the buffer content from the buffer with the length of 30
		buffer_length=read(_socketInfo.socketCon,buffer,30);


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
		sleep(1);
	}
	printf("receive data thread stop\n");
	return NULL;
}





void *create_and_listen(int port_num){
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
	
	/*
	here is the port num defination
	*/
	//define the port to be 2000
	server_addr.sin_port=htons(port_num);

	if(bind(socketlisten,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))!=0){
		perror("bind ip port fail ");
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

	pthread_t thread_accept;
	//pthread_create(&thread_accept,NULL,fun_therad_accept_handler,&socketlisten);
	pthread_create(&thread_accept,NULL,fun_therad_accept_handler,&socketlisten);
	//return thread_accept;

}




int main(int argc, char const *argv[])
{
	
	pthread_t already_create[5];
	int i=0;
	for(i=0;i<total_sock_num;i++){
		//pthread_t thread_accept=create_and_listen(2000+i);
		create_and_listen(2000+i);
		printf("create the %d link\n",2000+i);
		//already_create[i]=thread_accept;
	}

	//send the data all the time
	while(1){
		//judge how many thread is still alive
		int i=0;

		
		for(i=0;i<thread_receive_client_count;i++){
			if(check_thread_is_killed(Arr_Thread_Receive_Client[i])==1){
				printf("there is a thread been killed\n");
				thread_receive_client_count--;
			}
		}
		printf("now, the accept thread num is :%d\n",thread_receive_client_count);
		
		//get user operation ,and do it
		char user_str[30]="12345678901234567890123456789";
		

		printf("please server input:::::::\n");
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
			for(i=0;i<conClientCount;i++){
				int sendMsg_len=write(arr_socket[i].socketCon,user_str,30);
				if(sendMsg_len>0){
					printf("send to %s:%s success\n",arr_socket[i].ipaddr,arr_socket[i].port);
				}else{
					printf("send to %s:%s fail\n",arr_socket[i].ipaddr,arr_socket[i].port);
				}
			}
		}
		

		sleep(5);
	}

	printf("wait for thread quit\n");
	char* message;
	//int i=0;
	for(i=0;i<total_sock_num;i++){
		pthread_t thread_accept=already_create[i];
		pthread_join(thread_accept,(void *)&message);
	}

	printf("%s\n",message);

	return 0;
}




