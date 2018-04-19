#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
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

}

