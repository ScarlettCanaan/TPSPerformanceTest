#include "worker_instance.h"

//int ct = 0;
void worker_instance::pipe_connection(uv_stream_t *_server)
{
	client = (uv_pipe_t *)malloc(sizeof(uv_pipe_t));
	//client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	//std::cout << ct << std::endl;
	uv_pipe_init(loop, client, 0);
	//ct++;
	//std::cout << ct << std::endl;
	if (uv_accept(_server, (uv_stream_t *)client) == 0) {
		uv_write_t *req = new uv_write_t;
		buf = uv_buf_init("1", 1);
		int errorcode = uv_write2(req, (uv_stream_t *)&info.pipe, &buf, 1, (uv_stream_t *)client, NULL);
		if (errorcode < 0) error::PRINT_ERROR("sending message error", errorcode);
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

	std_c[0].flags = (uv_stdio_flags)(uv_stdio_flags::UV_CREATE_PIPE | uv_stdio_flags::UV_READABLE_PIPE);
	std_c[0].data.stream = (uv_stream_t *)(&info.pipe);
	std_c[1].flags = UV_IGNORE;
	std_c[2].flags = UV_INHERIT_FD;
	std_c[2].data.fd = 2;
	info.opt.stdio = std_c;
	info.opt.stdio_count = 3;
	info.opt.args = args;
	info.opt.file = args[0];
	info.opt.exit_cb = close_callback;
	info.opt.flags = NULL;
	info.opt.cwd = NULL;
	info.opt.uid = NULL;
	info.opt.env = NULL;
	
	int errorcode;
	errorcode = uv_spawn(loop, &info.process, &info.opt);
	if (errorcode < 0) error::PRINT_ERROR("spawning error", errorcode);
	std::cout << "Started worker [" << info.process.pid << "]" << std::endl;
}

int worker_instance::getPID()
{
	return info.process.pid;
}
worker_instance::worker_instance()
{
}


worker_instance::~worker_instance()
{
}
