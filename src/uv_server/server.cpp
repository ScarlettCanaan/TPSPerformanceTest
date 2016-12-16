#include "server.h"

static void* callback(void *arg, int status)
{
	((server*)arg)->accept_cb((uv_stream_t*)arg, status);
	return NULL;
}

void server::server_establish(const char *_ip, int _port)
{
	loop = uv_default_loop();
	struct sockaddr_in addr;
	errorcode = uv_ip4_addr(_ip, _port, &addr);
	if (errorcode < 0) error::PRINT_ERROR("obtaining ipv4 address error", errorcode);

	uv_tcp_init(loop, &_server);
	errorcode = uv_tcp_bind(&_server, (const struct sockaddr*) &addr, 0);
	if (errorcode < 0) error::PRINT_ERROR("binding server to socket error", errorcode);

	errorcode = uv_listen((uv_stream_t*)&_server, MAX_INCOMING_QUEUE_SIZE, (uv_connection_cb)callback);
	if (errorcode < 0) error::PRINT_ERROR("listening for connections error", errorcode);

	timer_watcher.addLoop(loop, 3000, 3000);
}

void server::accept_cb(uv_stream_t *server_stream, int status)
{
	multithreading = true;
	if (status < 0) {
		error::PRINT_ERROR("connection error", status);
	}
	server_conn_instance client;
	client.addThreadLoop((uv_stream_t*)server_stream);
}

void server::run(const char *_ip, int _port)
{
	server_establish(_ip, _port);
	uv_run(loop, UV_RUN_DEFAULT);
}

server* server::getInstance()
{
	static server SINGLETON;
	return &SINGLETON;
}

server::server()
{
}

server::~server()
{
}
