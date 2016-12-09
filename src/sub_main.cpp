#include <uv.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include "error.h"
#include "connect_info.h"

#define NOIPC	 0
#define IPC		 1

#define STDIN	 0
#define STDOUT	 1
#define STDERR	 2

uv_loop_t *loop;
uv_pipe_t queue;
uv_pipe_t pipeout;

int fd;
uv_fs_t req;

char path[100];
double cur_time;

uv_timer_t watcher;
int tick = 0;
long everagePacket = 0;
int errorcode;

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
	}
	else if (nread > 0) {
		connect_info::TotalSocketCountPlus();
		delete buf->base;
	}
	else if (nread < 0) {
		delete buf->base;
	}
}

void timer_cb(uv_timer_t * handle)
{
	uv_buf_t *buf = new uv_buf_t;
	int sc = connect_info::getTotalSocketCount();

	fprintf(stderr, "subprocess %d received %d packet on least 3 seconds! (%d sockets per second)\n", getpid(), sc, (int)(sc / 3));
	//char c_sc[30];
	//sprintf(c_sc, "%d", sc);
	//buf->base = c_sc;
	//buf->len = sizeof(c_sc);
	//errorcode = uv_fs_write(loop, &req, fd, buf, buf->len, -1, NULL);
	//if (errorcode < 0) error::PRINT_ERROR("writing error", errorcode);
	//offset += sizeof(info);
	//uv_fs_t close_req;
	//uv_fs_close(loop, &close_req, fd, NULL);
	//fd = uv_fs_open(loop, &req, path, O_CREAT | O_RDWR, 0644, NULL);
	connect_info::setTotalSocketCount(0);
	everagePacket += sc;
	delete buf;
	if (5 == ++tick)
	{
		//write packetCount to log file	
	}
}

void connect_cb(uv_stream_t *queue, ssize_t nread, const uv_buf_t *buf)
{
	fprintf(stderr, "Process [ %d ] accept a pipe connect\n", getpid());
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
	fprintf(stderr, "Worker %d: Accepted clinet fd %d\n", getpid(), client->io_watcher.fd);
	uv_read_start((uv_stream_t *)client, alloc_cb, read_cb);
}

int main()
{
	fprintf(stderr, "Worker %d established!\n", getpid());

	loop = uv_default_loop();
	errorcode = uv_pipe_init(loop, &queue, IPC);
	if (errorcode < 0 ) error::PRINT_ERROR("subprocess perform pipe error", errorcode);
	errorcode = uv_pipe_open(&queue, STDIN);
	if (errorcode < 0 ) error::PRINT_ERROR("subprocess perform open pipe error", errorcode);

	errorcode = uv_read_start((uv_stream_t *)&queue, alloc_cb, connect_cb);
	if (errorcode < 0 ) error::PRINT_ERROR("subprocess pipe readstart error", errorcode);

	connect_info::setTotalSocketCount(0);
	uv_timer_init(loop, &watcher);
	uv_timer_start(&watcher,  timer_cb, 3000, 3000);
	uv_run(loop, UV_RUN_DEFAULT);
	return 0;
}
