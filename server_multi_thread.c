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