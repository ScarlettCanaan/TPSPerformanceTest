#ifndef __TIMER_WATCHER_H__
#define __TIMER_WATCHER_H__

#include <uv.h>
#include <iostream>
#include "connect_info.h"

class TimerWatcher
{
public:
	TimerWatcher();
	~TimerWatcher();
	void addLoop(uv_loop_t* _loop, int _delay, int _repeat);
private:
	void inline timer_cb(uv_timer_t *_handle);

private:
	uv_timer_t watcher;
};


#endif // !__TIMER_WATCHER_H__
