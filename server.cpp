#include <ev.h>  
#include <stdio.h>  
#include <netinet/in.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>
//#include "commonport.h"

#define PORT 54674

const int BUFFER_SIZE = 2048;
const int MAX_ALLOWED_CLIENT = 10240;
  
struct ev_io *libevlist[MAX_ALLOWED_CLIENT] = {NULL};  
int threadCount = 0;
  
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);  
  
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);  
  
void timer_print(struct ev_loop *loop, struct ev_timer *watcher, int revents);  
  
int freelibev(struct ev_loop *loop, int fd);  
  
  
int main()  
{  
    struct ev_loop *loop = ev_default_loop(0);  
    int sd;  
    struct sockaddr_in addr;  
    int addr_len = sizeof(addr);  
    
    //create default event timer and io water 
    struct ev_io socket_accept;  
    struct ev_timer timeout_w;  
    
    //create socket connection
    sd = socket(PF_INET, SOCK_STREAM, 0);  
    if(sd < 0)  
    {  
        printf("socket error\n");  
        return -1;  
    }  
    bzero(&addr, sizeof(addr));  
    addr.sin_family = AF_INET;  
    addr.sin_port = htons(PORT);  
    addr.sin_addr.s_addr = INADDR_ANY;  
    
    //set bind
    if(bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0)  
    {  
        printf("bind error\n");  
        return -1;
    }

    //set listen  
    if(listen(sd, SOMAXCONN) < 0)  
    {  
        printf("listen error\n");  
        return -1;  
    }  
    
    //set fd reuse  
    int bReuseaddr=1;  
    if(setsockopt(sd,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(bReuseaddr)) != 0)  
    {  
        printf("setsockopt error in reuseaddr[%d]\n", sd);  
        return 0;  
    }  
      
    //initilal io watcher to set fd monitor 
    ev_io_init(&socket_accept, accept_cb, sd, EV_READ);  
    ev_io_start(loop, &socket_accept);  
      
    ev_timer_init(&timeout_w, timer_print, 3, 3);  
    ev_timer_start(loop, &timeout_w);  
      
    ev_run(loop, 0);  
      
    return 1;  
}  
  
//acceprt callback
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)  
{  

    struct sockaddr_in client_addr;  
    socklen_t client_len = sizeof(client_addr);  
    int client_sd;  
    //create client io watcher
    struct ev_io *w_client = (struct ev_io*) malloc(sizeof(struct ev_io));  
      
    if(w_client == NULL)  
    {  
        printf("malloc error in accept_cb\n");  
        return ;  
    }  
      
    if(EV_ERROR & revents)  
    {  
        printf("error event in accept\n");  
        return ;  
    }  
      
    //get connected client fd
    client_sd = accept(watcher->fd, (struct sockaddr*)&client_addr, &client_len);  
    if(client_sd < 0)  
    {  
        perror("accept error\n");  
        return;  
    }

    if( client_sd > MAX_ALLOWED_CLIENT)  
    {  
        printf("fd too large[%d]\n", client_sd);  
        close(client_sd);  
        return ;  
    }  
      
    if(libevlist[client_sd] != NULL)  
    {  
        printf("client_sd not NULL fd is [%d]\n", client_sd);  
        return ;  
    }  
      
    //printf("client connected client thread [%d]\n", client_sd); 

    //establish new connection 
    ev_io_init(w_client, read_cb, client_sd, EV_READ);  
    ev_io_start(loop, w_client);  
      
    libevlist[client_sd] = w_client;  
  
}  
  
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)  
{  
    char buffer[BUFFER_SIZE];  
    ssize_t read;  
      
    if(EV_ERROR & revents)  
    {  
        perror("error event in read\n");  
        return ;  
    }  

    read = recv(watcher->fd, buffer, BUFFER_SIZE, 0);  
    if(read < 0)  
    {  
        perror("read error\n");  
        return;  
    }  
      
    if(read == 0)  
    {  
        //printf("client thread [%d] disconnected.\n", watcher->fd);  
        freelibev(loop, watcher->fd);  
        return;  
    }  
    else  
    {
        threadCount++;
    }
    //send(watcher->fd, buffer, read, 0);  
    //bzero(buffer, read);  
}  
  
void timer_print(struct ev_loop *loop, struct ev_timer *watcher, int revents)  
{  
    if(EV_ERROR & revents)  
    {  
        printf("error event in timer_print\n");  
        return ;  
    }  
    printf("%d datagram has received on last 3 seconds\n", threadCount); 
    threadCount = 0;
    return;  
}  
  
int freelibev(struct ev_loop *loop, int fd)  
{     
    if(libevlist[fd] == NULL)  
    {  
        printf("the fd already freed[%d]\n", fd);  
        return -1;  
    }
    close(fd);  
    ev_io_stop(loop, libevlist[fd]);  
    free(libevlist[fd]);  
    libevlist[fd] = NULL;  
    return 1;  
}  