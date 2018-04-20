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


//using namespace std;
/*
htons:host to network short
htonl:host to network long
ntohs:network to host short
ntohl:network to host long

in_addr_t inet_addr(const char* strptr)
//turn the char of ip address to the struct of ipv4 address in linux

char* inet_ntoa(struct in_addr* addrptr)
//turn the ipv4 struct of ipv4 address to char 

struct hostent* gethostbyname(const char* name)
//turn the hostname to ip

*/

#define portnumber 3333

int main(int argc,char *argv[]){
	printf("hello\n");

	int sockfd,new_fd;
	struct  sockaddr_in server_addr;//server address
	struct  sockaddr_in client_addr;
	int sin_size;
	char hello[]="hello this is kiwi server\n";

	/*************************** create **************************/
	//server create the socket desciper
	// FIRST IS PROTOCOL FAMILY
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){//AF_INET is ipv4;sock_stream : tcp
		printf("socket error is %s\n\a",strerror(errno));
		fprintf(stderr,"socket error is %s\n\a",strerror(errno));
		exit(1);
	}


	/*************************** init  **************************/
	/* 
	server full the sockaddr struct
	*/
	//init the server_addr to 0
	bzero(&server_addr,sizeof(struct  sockaddr_in));

	server_addr.sin_family=AF_INET;//the famuly is internet

	//turn local host long data to network long data
	//INADDR_ANY means can receive any ip address,also means can bind to any ip
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	//server_addr.sin_addr=inet_addr("192.168.10.1");// bind the address struct to a stubbern ip address
	// use the inet_addr to transfer ip to the interger ip(the liunx ip)

	
	//turn the host short data to network short data (the port number)
	server_addr.sin_port=htons(portnumber);

	/*************************** bind **************************/
	if(bind(sockfd,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr))==-1){
		printf("bind error is %s\n\a",strerror(errno));
		fprintf(stderr,"bind error is %s\n\a",strerror(errno));
		exit(1);
	}

	/*************************** listen **************************/
	if(listen(sockfd,5)==-1){
		//cout<<" listen error:"<<strerror(errno)<<endl;
		printf("listen error is %s\n\a",strerror(errno));
		fprintf(stderr,"listen error is %s\n\a",strerror(errno));
		exit(1);
	}

	/*************************** listening to the clinet **************************/
	int count=0;
	while(1){
		//block the server till the clinet connect to the server
		count++;
		printf("now begin listen %d times",count);
		sin_size=sizeof(struct sockaddr_in);
		
		if((new_fd=accept(sockfd,(struct sockaddr*)&client_addr,&sin_size))==-1){//not connect successfully
			//cout<<" connect err:"<<strerror(errno)<<endl;
			printf("connect error is %s\n\a",strerror(errno));
			fprintf(stderr,"connect error is %s\n\a",strerror(errno));
			exit(1);
		}
		//turn the clinet address from inter to char and print out;
		//cout<<" server get connection from "<<inet_ntoa(client_addr.sin_addr)<<endl;
		printf("server get connect to client: %s\n\a",inet_ntoa(client_addr.sin_addr));
		fprintf(stderr,"server get connection from %s\n\a",inet_ntoa(client_addr.sin_addr));

		if(write(new_fd,hello,strlen(hello))==-1){
			//cout<<"write error : "<<strerror(errno)<<endl;
			printf("write error is %s\n\a",strerror(errno));
			fprintf(stderr,"write error is %s\n\a",strerror(errno));
			exit(1);
		}
		close(new_fd);
	}

	close(sockfd);
	exit(0);
	return 1;
}