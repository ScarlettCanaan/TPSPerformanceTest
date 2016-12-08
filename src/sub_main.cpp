#include <uv.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include "error.h"
#include "connect_info.h"

uv_loop_t *loop;
uv_pipe_t queue;
uv_pipe_t pipeout;

int fd;
uv_fs_t req;

char path[100];
double cur_time;

	uv_timer_t watcher;
int offset = 0;

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
//	char info[500];
//	uv_uptime(&cur_time);
//	gcvt(cur_time, 5, info);
//	strcat(info, " 3 seconds received ");
	int sc = connect_info::getTotalSocketCount();

	std::cout<< "Total count:" << sc << std::endl;
	char c_sc[30];
	sprintf(c_sc, "%d", sc);
//	strcat(info, c_sc);
//	strcat(info, " packets");
	buf->base = c_sc;
//	std::cout << buf->base << std::endl;
	uv_fs_t read_req;
	errorcode = uv_fs_write(loop, &read_req, fd, buf, 8, -1, NULL);
	if (errorcode < 0) error::PRINT_ERROR("writing error", errorcode);
//	offset += sizeof(info);
	uv_fs_t close_req;
	uv_fs_close(loop, &close_req, fd, NULL);
	fd = uv_fs_open(loop, &req, path, O_CREAT | O_RDWR, 0644, NULL);
	delete buf;
}

void connect_cb(uv_stream_t *queue, ssize_t nread, const uv_buf_t *buf)
{
	std::cout << "Process [" << getpid() <<"] accept a pipe connect" << std::endl;
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


	errorcode = uv_timer_start(&watcher, (uv_timer_cb) timer_cb, 3000, 3000);
	if (errorcode < 0 ) error::PRINT_ERROR("subprocess timerstart error", errorcode);
}

int main()
{
	memset(path, 0, sizeof(path));
	int pid = getpid();
	sprintf(path, "%d", pid);
	strcat(path, "pid_process");

	loop = uv_default_loop();
	errorcode = uv_pipe_init(loop, &queue, 1);
	if (errorcode < 0 ) error::PRINT_ERROR("subprocess perform pipe error", errorcode);
	errorcode = uv_pipe_init(loop, &pipeout, 0);
	if (errorcode < 0 ) error::PRINT_ERROR("subprocess perform pipe error", errorcode);
	errorcode = uv_timer_init(loop, &watcher);
	if (errorcode < 0 ) error::PRINT_ERROR("subprocess init timer error", errorcode);
	errorcode = uv_pipe_open(&queue, 0);
	if (errorcode < 0 ) error::PRINT_ERROR("subprocess perform open pipe error", errorcode);

	errorcode = uv_pipe_open(&pipeout, 1);
	if (errorcode < 0 ) error::PRINT_ERROR("subprocess perform open pipe error", errorcode);

	fd = uv_fs_open(loop, &req, path, O_CREAT | O_RDWR, 0644, NULL);
	std::cout << "subprocess" << getpid() << "establishd" << std::endl;

	errorcode = uv_read_start((uv_stream_t *)&queue, alloc_cb, connect_cb);
	if (errorcode < 0 ) error::PRINT_ERROR("subprocess pipe readstart error", errorcode);

	
	connect_info::setTotalSocketCount(0);
	return uv_run(loop, UV_RUN_DEFAULT);
}
