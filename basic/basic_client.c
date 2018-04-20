#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>

//#define portnumber 3333
#define sock_total_num 1

int client_get_connect(char *ip,int port_num){
	int sockfd;
	char buffer[1024];
	struct sockaddr_in server_addr;
	struct hostent *host;
	int nbytes;

	printf("IP ADDR IS %s\n",ip);

	if((host=gethostbyname(ip))==NULL){
		fprintf(stderr, "gethostbyname error\n");
		exit(1);
	}


	/******************************create the socket********************************/
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		fprintf(stderr," socker error %s\n",strerror(errno));
		exit(-1);
	}

	/*****fill the client infomation******/
	bzero(&server_addr,sizeof(server_addr));//init the server_addr to zero
	server_addr.sin_family=AF_INET;//use ipv4 
	
	//server_addr.sin_port=htons(portnumber);
	server_addr.sin_port=htons(port_num);

	server_addr.sin_addr=*((struct in_addr *)host->h_addr);

	//string server address = server_addr.sin_addr;
	//printf("the server add is \n");
	if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1){
		fprintf(stderr, " connect error %s\n", strerror(errno));
		exit(1);
	}

	/**************** connect suucessfully ****************/
	if((nbytes=read(sockfd,buffer,1024))==-1){
		fprintf(stderr, " read error %s\n", strerror(errno));
		exit(-1);
	}

	buffer[nbytes]='\0';
	printf("get handshake : %s and waiting.....\n",buffer);
	



	return sockfd;

	//close(sockfd);
	
}



int while_listen(int sockfd){
	char buffer[1024];
	int nbytes;
	while(1){
		if((nbytes=read(sockfd,buffer,1024))==-1){
			fprintf(stderr, " read error %s\n", strerror(errno));
			exit(-1);
		}

		buffer[nbytes]='\0';
		printf("get res: .....\n",buffer);
		if(buffer=="end"){
			printf("get end and exit\n");
			break;
		}

		sleep(2);
	}
}

int main(int argc, char const *argv[])
{	
	if(argc!=2){
		fprintf(stderr, "usage %s hostname\n", argv[0]);
		exit(1);
	}

	char user_str[30]="127.0.0.1";
	/*
	printf("please server input:::::::\n");
		scanf("%s",user_str);
	*/
	int sock_vector[sock_total_num];
	int i=0;
	for(i=0;i<sock_total_num;i++){
		int sock_fd=client_get_connect(user_str,3333+i);
		sock_vector[i]=sock_fd;
	}
	
	while_listen(sock_vector[0]);

	sleep(100);
	for(i=0;i<sock_total_num;i++){
		int sock_fd=sock_vector[i];
		close(sock_fd);
	}
	
	return 0;
}
