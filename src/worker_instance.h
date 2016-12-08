#ifndef __WORKER_INSTANCE_H__
#define __WORKER_INSTANCE_H__

#include <uv.h>
#include "error.h"

class worker_instance
{
public:
	void close_cb(uv_process_t* _process, int64_t exit_status, int term_signal);

public:
	void setupWorker(uv_loop_t *_loop, char *path);
	void pipe_connection(uv_stream_t *_server);
	int getPID();
public:
	worker_instance();
	~worker_instance();

private:
	struct process_info {
		uv_process_t process;
		uv_process_options_t opt;
		uv_pipe_t pipe;
	} info;
	uv_stdio_container_t std_c[3];
	uv_pipe_t *client;
	uv_loop_t *loop;
	uv_buf_t buf;
};
#endif // !__WORKER_INSTANCE_H__

#include <uv.h>


