#include "server_conn_instance.h"

static void* accept_callback(void* arg)
{
	((server_conn_instance*)arg)->client_accept(arg);
	return NULL;
}

static void* alloc_callback(uv_handle_t *handle, size_t size, uv_buf_t *buf)
{
	((server_conn_instance*)handle)->alloc_cb(handle, size, buf);
	return NULL;
}

static void* read_callback(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	((server_conn_instance*)client)->read_cb(client, nread, buf);
	return NULL;
}

void server_conn_instance::addThreadLoop(uv_stream_t *server)
{

	connect_info::ThreadCountPlus();
	int errorcode;
	errorcode = uv_thread_create(&client_thread, (uv_thread_cb)accept_callback, server);
	if (errorcode < 0) error::PRINT_ERROR("thread_create error", errorcode);
}

void server_conn_instance::client_accept(void *entry)
{
	int errorcode;
	loop = new uv_loop_t;
	uv_loop_init(loop);
	client = new uv_tcp_t;
	uv_tcp_init(loop, client);

	errorcode = uv_accept((uv_stream_t*)entry, (uv_stream_t*)client);
	if (errorcode < 0) {
		uv_close((uv_handle_t*)client, NULL);
		delete client;
		delete loop;
		error::PRINT_ERROR("error accepting connection", errorcode);
	}
	else {
		connect_info::AcceptCountPlus();
		uv_read_start((uv_stream_t*)client, (uv_alloc_cb)alloc_callback, (uv_read_cb)read_callback);
	}

	uv_run(loop, UV_RUN_DEFAULT);
}

//malloc callback
void server_conn_instance::alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf)
{
	*buf = uv_buf_init((char*)malloc(size), size);
	assert(buf->base != NULL);
}

//read callback
void server_conn_instance::read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	if (nread == UV_EOF) {
		connect_info::AcceptCountMinus();
		connect_info::ThreadCountMinus();
		free(buf->base);
		uv_close((uv_handle_t*)client, NULL);
		delete client;
		delete loop;
	}
	else if (nread > 0) {
		connect_info::TotalSocketCountPlus();
		free(buf->base);
		if (feedback) {
			//	write_req_t *wrt;
			// 	wrt = (write_req_t*) malloc(sizeof(write_req_t));
			//	wrt->buf = uv_buf_init(buf->base, nread);
			//	uv_write(&wrt->req, client, &wrt->buf, 1, write_cb);
		}
	}
	else if (nread == 0) {
		free(buf->base);
	}
}

//write_callback
//void server_conn_instance::write_cb(uv_write_t *req, int status) {
//	if (status) {
//		PRINT_ERROR("async write error", status);
//	}
//	write_req_t* wrt;
//	wrt = (write_req_t*) req;
//	assert(wrt->req.type == UV_WRITE);
//	++packetCount;
//	free(wrt->buf.base);
//	free(wrt);
//}

server_conn_instance::server_conn_instance()
{
}


server_conn_instance::~server_conn_instance()
{
}
