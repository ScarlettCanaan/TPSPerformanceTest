#ifndef __SERVER_H__
#define __SERVER_H__

#include <iostream>
#include <stdio.h>  
#include <uv.h>
#include <stdlib.h>  
#include <string.h>
#include <iostream>
#include <assert.h>
#include <list>
#include "server_conn_instance.h"
#include "TimerWatcher.h"
#include "error.h"

const int MAX_INCOMING_QUEUE_SIZE = 10240;

class server
{

public:
	static server* getInstance();

private:
	server();
	server(const server &);
	server& operator = (const server &);
	virtual ~server();
	void server_establish(const char *_ip, int _port);
public:
	void accept_cb(uv_stream_t *server, int status);

public:
	void run(const char *_ip, int _port);

private:
	uv_loop_t *loop;
	uv_tcp_t _server;
	TimerWatcher timer_watcher;
	std::list<server_conn_instance*> acceptList;
	int errorcode;
	bool multithreading;
};


#endif // !__SERVER_H__

