#ifndef __SERVER_CONN_INSTANCE_H__
#define __SERVER_CONN_INSTANCE_H__

#include <uv.h>
#include <assert.h>
#include <stdlib.h>
#include "error.h"
#include "connect_info.h"

class server_conn_instance
{
public:
	server_conn_instance();
	~server_conn_instance();
	void addThreadLoop(uv_stream_t *server);

private:
public:
	void client_accept(void *entry);
	void read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
	void write_cb(uv_write_t *req, int status);
	void alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf);

private:
	uv_thread_t client_thread;
	uv_loop_t *loop;
	uv_tcp_t *client;
	bool feedback = false;
};


#endif // !__SERVER_CONN_INSTANCE_H__

