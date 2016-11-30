#include <iostream> 
#include <netinet/in.h>  
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <stdio.h>
#include <ev.h>
#define PORT 54674

const int MAX_THREAD_COUNT = 20;
int datapacketCount = 0;

void* socketDispatch(void *arg);  
void timer_print(struct ev_loop *loop, struct ev_timer *watcher, int revents);

int main()  
{  
	char i = 0;
    for (int i = 0; i < MAX_THREAD_COUNT; ++i)
    {
    	pthread_t sniffer_thread;
    	if (pthread_create( &sniffer_thread, NULL, &socketDispatch, NULL) < 0)
    	{
    		std::cout << "create thread faild" << std::endl;
    	}
    }
    struct ev_loop *loop = ev_default_loop(0);
    struct ev_timer timeout_w;

    ev_timer_init(&timeout_w, timer_print, 3, 3);  
    ev_timer_start(loop, &timeout_w); 
    ev_run(loop, 0);
    return 1;  
}  

void* socketDispatch(void *arg)
{
	int sd;
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	char token[1];
	token[0] = 'a';

	bzero(&addr, addr_len);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "socket faild" << std::endl;
		return NULL;
	}
	if (connect(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("connect faild\n");
		//std::cout << "connect faild" << std::endl;
		return NULL;
	}
	while(1)
	{
		if (send(sd, token, 1, 0) < 0)
		{
			std::cout << "send message faild" << std::endl;
			return NULL;
		}
		else
		{
			++datapacketCount;
		}
	}
	close(sd);
}

void timer_print(struct ev_loop *loop, struct ev_timer *watcher, int revents)  
{  
    if(EV_ERROR & revents)  
    {  
        printf("error event in timer_print\n");  
        return ;  
    }  
    printf("%d datagram has received on last 3 seconds\n", datapacketCount); 
    datapacketCount = 0;
    return;  
}  
