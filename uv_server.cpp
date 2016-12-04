#include <stdio.h>  
#include <uv.h>
#include <stdlib.h>  
#include <string.h>
#include <iostream>
#include <assert.h>

//#define PORT 54674

const int BUFFER_SIZE = 2048;
const int MAX_ALLOWED_CLIENT = 10240;
const int feedBack = 0;

static long packetCount = 0;
static int PORT = 0;
//uv_loop_t *loop;

void accept_cb(uv_stream_t *server, int status);  
  
void client_accept(void *args);

void read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
  
void write_cb(uv_write_t *req, int status); 
	
void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf);

void timer_print(uv_timer_t *handle);

inline void PRINT_ERROR(const char *msg, const int r);

int main(int argc, char* argv[])  
{
	if (argc != 2) {
		PRINT_ERROR("invalid args", argc);
	}
	PORT = atoi(argv[1]);
	int r;
	uv_loop_t *loop = uv_default_loop();
	
    struct sockaddr_in addr;
	r = uv_ip4_addr("0.0.0.0", PORT, &addr);
    if (r) PRINT_ERROR("obtaining ipv4 address error", r);

	uv_tcp_t server;
	uv_tcp_init(loop, &server);
	r = uv_tcp_bind(&server, (const struct sockaddr*) &addr, 0);
    if (r) PRINT_ERROR("binding server to socket error", r);
    
	r = uv_listen((uv_stream_t*) &server, MAX_ALLOWED_CLIENT, accept_cb);
	if (r) PRINT_ERROR("listening for connections error", r);
      
	uv_timer_t watcher;
	uv_timer_init(loop, &watcher);
	uv_timer_start(&watcher, timer_print, 3000, 3000);
    uv_run(loop, UV_RUN_DEFAULT); 
    return 0;  
}  
  
//accept callback
void accept_cb(uv_stream_t *server, int status)  
{  
	int r;
	if (status) {
		PRINT_ERROR("connection error", status);
	}

	uv_thread_t client_thread;
	r = uv_thread_create(&client_thread, client_accept, server);
	if (r) PRINT_ERROR("thread_create error", r);

	r = uv_thread_join(&client_thread);
}

//client_entry
void client_accept(void *entry)
{
	int r;	
	uv_loop_t *loop = uv_default_loop();
	uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);

	r = uv_accept( (uv_stream_t*) entry, (uv_stream_t*) client);
	if (r) {
		PRINT_ERROR("error accepting connection %d", r);
		uv_close((uv_handle_t*) client, NULL);
	} else {
		uv_read_start((uv_stream_t*) client, alloc_cb, read_cb);
	}

	uv_run(loop, UV_RUN_DEFAULT);
}
//malloc callback
void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf)
{
	*buf = uv_buf_init((char*) malloc(size), size);
	assert(buf->base != NULL);
}	

//read callback
void read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{ 
	if (nread == UV_EOF) {
		free(buf->base);
		uv_close((uv_handle_t*) client, NULL);
	}
	else if (nread > 0) {
		++packetCount;
		free(buf->base);
		if (feedBack) {
		//	write_req_t *wrt;
		// 	wrt = (write_req_t*) malloc(sizeof(write_req_t));
		//	wrt->buf = uv_buf_init(buf->base, nread);
		//	uv_write(&wrt->req, client, &wrt->buf, 1, write_cb);
		}		
	} else if (nread == 0) {
		free(buf->base);
	}
}  

//write_callback
//void write_cb(uv_write_t *req, int status) {
//	if (status) {
//		PRINT_ERROR("async write error", status);
//	}
//	write_req_t* wrt;
//	wrt = (write_req_t*) req;
//	assert(wrt->req.type == UV_WRITE);
//	++packetCount;
//	free(wrt->buf.base);
//	free(wrt);
//}

void timer_print(uv_timer_t *handle)  
{  
	std::cout << packetCount << " datagram has received on least 3 seconds" << std::endl;
    packetCount = 0;
    return ;  
}  
  
inline void PRINT_ERROR(const char *msg, const int r) 
{
	do {
		std::cerr << msg << ": " << strerror(errno) << std::endl;
		assert(0);
	} while(0);
}
