#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>

#define portnumber 3333

int main(int argc, char const *argv[])
{
	int sockfd;
	char buffer[1024];
	struct sockaddr_in server_addr;//the server address
	struct hostent *host;
	int nbytes;

	//use the hostname to qurify the host
	if(argc!=2){
		fprintf(stderr, "usage %s hostname\n", argv[0]);
		exit(1);
	}

	if((host=gethostbyname(argv[1]))==NULL){
		fprintf(stderr, "gethostbyname error\n");
		exit(1);
	}

	/******************************create the socket********************************/
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		fprintf(stderr, " socket errno %s\n",strerror(errno));
		exit(1);
	}


	/*****fill the client infomation******/
	bzero(&server_addr,sizeof(server_addr));//init the server_addr to zero
	server_addr.sin_family=AF_INET;//use ipv4 
	server_addr.sin_port=htons(portnumber);
	server_addr.sin_addr=*((struct in_addr *)host->h_addr);

	//string server_address=server_addr.sin_addr;
	printf("the server add is  \n");
	if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1){
		fprintf(stderr, " connect error %s\n", strerror(errno));
		exit(1);
	}

	/**************** connect suucessfully ****************/
	if((nbytes=read(sockfd,buffer,1024))==-1){
		fprintf(stderr, " read error  %s\n", strerror(errno));
		exit(1);
	}

	buffer[nbytes]='\0';
	printf("get the buffer: %s\n",buffer);

	close(sockfd);
	exit(0);
	return 0;
}