#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h> 
#include <stdlib.h>
#include <stdio.h> 
//#include <iostream>

#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_nota()
//服务器端

#define port_number 3333

int sock_get(){
	int sockfd,new_fd;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sin_size;
	char hello[]="hello this is kiwi server\n";
	/***************************create*************************************/
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){//AF_INET:IPV4	SOCK_STREAm:tcp
		printf("socket error is %s\n\a", strerror(errno));
		fprintf(stderr, "socket error is %s\n\a", strerror(errno));
		exit(-1);
	}

	/***************************init*************************************/
	bzero(&server_addr,sizeof(struct sockaddr_in));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(port_number);


	/***************************bind*************************************/
	if(bind(sockfd,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr))==-1){
		printf("bind error is %s\n\a", strerror(errno));
		fprintf(stderr, "bind error is %s\n\a", strerror(errno));
		exit(1);
	}

	/***************************listen*************************************/
	if(listen(sockfd,5)==-1){
		printf("listen error is %s\n\a",strerror(errno));
		fprintf(stderr, "listen error is %s\n\a", strerror(errno));
		exit(1);
	}

	/***************************listen to client*************************************/
	int count=0;
	while(1){
		count++;
		printf("begin listen %d times\n", count);
		sin_size=sizeof(struct sockaddr_in);

		if((new_fd=accept(sockfd,(struct sockaddr*)&client_addr,&sin_size))==-1){
			printf("connect error is %s\n\a", strerror(errno));
			fprintf(stderr, "connect error is %s\n\a", strerror(errno));
			exit(1);
		}
	
		printf("server get connect to client: %s\n\a", inet_ntoa(client_addr.sin_addr));
		fprintf(stderr, "server get connetion from %s\n\a", inet_ntoa(client_addr.sin_addr));
	
		if(write(new_fd,hello,strlen(hello))==-1){
			printf("write errno is %s\n\a", strerror(errno));
			fprintf(stderr, "write error is %s\n\a", strerror(errno));
			exit(1);
		}
		close(new_fd);
	}
	close(sockfd);
	exit(0);
	return 1;
}



int main(int argc, char const *argv[])
{
	sock_get();
	return 0;
}