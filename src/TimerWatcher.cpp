#include "TimerWatcher.h"

void inline TimerWatcher::timer_cb(uv_timer_t *_handle)
{
	std::cout << "##############Server##################" << std::endl;
	std::cout << "[" << connect_info::getTotalSocketCount() << "] datagram has received" << std::endl;
	std::cout << "[" << connect_info::getAcceptCount() << "] tcp accept established." << std::endl;
	std::cout << "[" << connect_info::getThreadCount() << "] thread running			on least 3 seconds." << std::endl;
	connect_info::setTotalSocketCount(0);
	return;
}

void TimerWatcher::addLoop(uv_loop_t* _loop, int _delay, int _repeat)
{
	uv_timer_init(_loop, &watcher);
	uv_timer_start(&watcher, (uv_timer_cb)&TimerWatcher::timer_cb, _delay, _repeat);
}

TimerWatcher::TimerWatcher()
{
	connect_info::setAcceptCount(0);
	connect_info::setThreadCount(0);
	connect_info::setTotalSocketCount(0);
}


TimerWatcher::~TimerWatcher()
{
}
