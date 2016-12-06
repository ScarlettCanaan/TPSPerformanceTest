#include "server.h"

//static void* callback(void* arg, uv_stream_t *_server, int status)
//{
//	((server*)arg)->accept_cb(_server, status);
//	return NULL;
//}

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

//	std::cout << "binding..." << std::endl;
	//errorcode = uv_listen((uv_stream_t*)&_server, MAX_ALLOWED_CLIENT, (uv_connection_cb)&server::accept_cb);
	errorcode = uv_listen((uv_stream_t*)&_server, MAX_INCOMING_QUEUE_SIZE, (uv_connection_cb)callback);
//	std::cout << "listeniing..." << std::endl;
	if (errorcode < 0) error::PRINT_ERROR("listening for connections error", errorcode);

	timer_watcher.addLoop(loop, 3000, 3000);
//	std::cout << "add loop success" << std::endl;
}

void server::accept_cb(uv_stream_t *server_stream, int status)
{
	multithreading = true;
	if (status < 0) {
		error::PRINT_ERROR("connection error", status);
	}
	server_conn_instance client;
	//	acceptList.push_back(&client);
	//	std::cout << "add 1 thread loop" << std::endl;
	client.addThreadLoop((uv_stream_t*)server_stream);
	//	std::cout << "accept 1 client" << std::endl;
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
