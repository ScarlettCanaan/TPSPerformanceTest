#include "multi_process_server.h"

//static void* callback(void* arg, uv_stream_t *_server, int status)
//{
//	((server*)arg)->accept_cb(_server, status);
//	return NULL;
//}
char PATH[] = "./sub_main";
static void* callback(void *arg, int status)
{
	((multi_process_server*)arg)->accept_cb((uv_stream_t*)arg, status);
	return NULL;
}

void multi_process_server::server_establish(const char *_ip, int _port)
{	
	int cpu_count;
	uv_cpu_info_t *cpu_info;

	loop = uv_default_loop();
	uv_cpu_info(&cpu_info, &cpu_count);
	max_worker_count = cpu_count;
	uv_free_cpu_info(cpu_info, cpu_count);
	cpu_count++;
	workers = new worker_instance[cpu_count];
	while (cpu_count--) {
		workers[cpu_count].setupWorker(loop, PATH);
		connect_info::ThreadCountPlus();
	}

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

void multi_process_server::accept_cb(uv_stream_t *server_stream, int status)
{
	if (status < 0) {
		error::PRINT_ERROR("connection error", status);
	}
	workers[round_robin_count].pipe_connection(server_stream);
	round_robin_count = (round_robin_count + 1) % max_worker_count;
	connect_info::AcceptCountPlus();
}

void multi_process_server::run(const char *_ip, int _port)
{
	connect_info::setAcceptCount(0);
	connect_info::setThreadCount(0);
	connect_info::setTotalSocketCount(0);
	server_establish(_ip, _port);
	uv_run(loop, UV_RUN_DEFAULT);
}

multi_process_server* multi_process_server::getInstance()
{
	static multi_process_server SINGLETON;
	return &SINGLETON;
}

multi_process_server::multi_process_server()
{
}

multi_process_server::~multi_process_server()
{
}
