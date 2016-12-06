#include <uv.h>
#include "error.h"
#include "connect_info.h"

uv_loop_t *loop;
uv_pipe_t queue;

void alloc_cb(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = sizeof(buf->base);
}

void read_cb(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf)
{
	if (nread == UV_EOF)
	{
		uv_close((uv_handle_t *)handle, NULL);
		delete handle;
		delete buf->base;
		delete buf;
	}
	else if (nread > 0) {
		connect_info::TotalSocketCountPlus();
		delete buf->base;
		delete buf;
	}
	else if (nread < 0) {
		delete buf->base;
		delete buf;
	}
}

void connect_cb(uv_stream_t *queue, ssize_t nread, const uv_buf_t *buf)
{
	if (nread == UV_EOF) {
		uv_close((uv_handle_t *)queue, NULL);
		return;
	}
	uv_pipe_t *pipe = (uv_pipe_t *)queue;
	if (!uv_pipe_pending_count(pipe)) {
		error::PRINT_ERROR("No pending pipe" , 0);
	}

	uv_handle_type type = uv_pipe_pending_type(pipe);
	if (type != uv_handle_type::UV_TCP) {
		error::PRINT_ERROR("Invalid pipe type", 0);
	}

	uv_tcp_t *client = new uv_tcp_t;
	uv_tcp_init(loop, client);
	int errorcode;
	errorcode = uv_accept(queue, (uv_stream_t *)client);
	if (errorcode < 0) {
		uv_close((uv_handle_t *)client, NULL);
		error::PRINT_ERROR("error accepting connection", errorcode);
	}
	uv_read_start((uv_stream_t *)client, alloc_cb, read_cb);

}

int main()
{
	loop = uv_default_loop();
	uv_pipe_init(loop, &queue, 1);
	uv_pipe_open(&queue, 0);
	uv_read_start((uv_stream_t *)&queue, alloc_cb, connect_cb);
	return uv_run(loop, UV_RUN_DEFAULT);
}
