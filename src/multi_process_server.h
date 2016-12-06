#ifndef __MULTI_PROCESS_SERVER_H__
#define __MULTI_PROCESS_SERVER_H__

#include <iostream>
#include <stdio.h>  
#include <uv.h>
#include <stdlib.h>  
#include <string.h>
#include <iostream>
#include <assert.h>
#include <list>
#include "worker_instance.h"
#include "TimerWatcher.h"
#include "error.h"
#include "connect_info.h"

const int MAX_INCOMING_QUEUE_SIZE = 10240;

class multi_process_server
{

public:
	static multi_process_server* getInstance();

private:
	multi_process_server();
	multi_process_server(const multi_process_server &);
	multi_process_server& operator = (const multi_process_server &);
	virtual ~multi_process_server();
	void server_establish(const char *_ip, int _port);
public:
	void accept_cb(uv_stream_t *server, int status);

public:
	void run(const char *_ip, int _port);

private:
	uv_loop_t *loop;
	uv_tcp_t _server;
	TimerWatcher timer_watcher;
	worker_instance *workers;
	int max_worker_count;
	int round_robin_count = 0;
	int errorcode;
};


#endif // !__SERVER_H__

