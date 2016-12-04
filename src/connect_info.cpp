#include "connect_info.h"

long connect_info::acceptCount = 0;
long connect_info::threadCount = 0;
long connect_info::totalSocketCount = 0;

long connect_info::getAcceptCount()
{
	return acceptCount;
}

long connect_info::getThreadCount()
{
	return threadCount;
}

long connect_info::getTotalSocketCount()
{
	return totalSocketCount;
}

long connect_info::setAcceptCount(long _value)
{
	return (acceptCount = _value);
}

long connect_info::setThreadCount(long _value)
{
	return (threadCount = _value);
}

long connect_info::setTotalSocketCount(long _value)
{
	return (threadCount = _value);
}

long connect_info::AcceptCountPlus()
{
	return (++acceptCount);
}

long connect_info::ThreadCountPlus()
{
	return (++threadCount);
}

long connect_info::TotalSocketCountPlus()
{
	return (++threadCount);
}
