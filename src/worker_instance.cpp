#include "worker_instance.h"

void worker_instance::pipe_connection(uv_stream_t *_server)
{
	client = new uv_tcp_t;
	//client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	if (uv_accept(_server, (uv_stream_t *)client) == 0) {
		uv_write_t *req = new uv_write_t;
		buf = uv_buf_init("1", 1);
		uv_write2(req, (uv_stream_t *)&info.pipe, &buf, 1, (uv_stream_t *)client, NULL);
	} else {
		uv_close((uv_handle_t *)client, NULL);
		delete client;
	}
}

static void close_callback(uv_process_t *_process, int64_t exit_status, int term_signal)
{
	((worker_instance*)(_process->data))->close_cb(_process, exit_status, term_signal);
}

void worker_instance::close_cb(uv_process_t *_process, int64_t exit_status, int term_signal)
{
	fprintf(stderr, "Process exited with status %d , signal %d\n", exit_status, term_signal);
	uv_close((uv_handle_t*)&info.process, NULL);
}

void worker_instance::setupWorker(uv_loop_t *_loop, char *path)
{
	char *args[2];
	args[0] = path;
	args[1] = NULL;

	loop = _loop;
	uv_pipe_init(loop, &(info.pipe), 1);

	//set handle to instance for callback encapsulation
	info.process.data = this;

	std_c.flags = (uv_stdio_flags)(uv_stdio_flags::UV_CREATE_PIPE | uv_stdio_flags::UV_READABLE_PIPE);
	std_c.data.stream = (uv_stream_t *)(&info.pipe);
	info.opt.stdio = &std_c;
	info.opt.stdio_count = 1;
	info.opt.args = args;
	info.opt.file = args[0];
	info.opt.exit_cb = close_callback;
	
	uv_spawn(loop, &info.process, &info.opt);
	std::cout << "Started worker [" << info.process.pid << "]" << std::endl;
}

worker_instance::worker_instance()
{
}


worker_instance::~worker_instance()
{
}
