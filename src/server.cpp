#include "server.h"

void server::server_establish(const char *_ip, int _port)
{
	loop = uv_default_loop();
	struct sockaddr_in addr;
	errorcode = uv_ip4_addr(_ip, _port, &addr);
	if (errorcode) error::PRINT_ERROR("obtaining ipv4 address error", errorcode);

	uv_tcp_init(loop, &_server);
	errorcode = uv_tcp_bind(&_server, (const struct sockaddr*) &addr, 0);
	if (errorcode) error::PRINT_ERROR("binding server to socket error", errorcode);

	errorcode = uv_listen((uv_stream_t*)&_server, MAX_ALLOWED_CLIENT, (uv_connection_cb)&server::accept_cb);
	if (errorcode) error::PRINT_ERROR("listening for connections error", errorcode);

	timer_watcher.addLoop(loop, 3000, 3000);
}

void server::accept_cb(uv_stream_t *server, int status)
{
	if (status) {
		error::PRINT_ERROR("connection error", status);
	}
	if (acceptList.size() >= MAX_ALLOWED_CLIENT) {
		error::PRINT_ERROR("thread count overflow", 0);
	}

	server_conn_instance client;
	acceptList.push_back(client);
	client.addThreadLoop((uv_stream_t*)&_server);
	std::cout << "accept 1 client" << std::endl;
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
