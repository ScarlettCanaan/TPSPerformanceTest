#ifndef __CONNECT_H__
#define __CONNECT_H__

class connect_info
{
private:
	connect_info() {};
	~connect_info() {};

public:
	static long getAcceptCount();
	static long getThreadCount();
	static long getTotalSocketCount();
	static long setAcceptCount(long _value);
	static long setThreadCount(long _value);
	static long setTotalSocketCount(long _value);
	static long AcceptCountPlus();
	static long ThreadCountPlus();
	static long TotalSocketCountPlus();
private:
	static long acceptCount;
	static long threadCount;
	static long totalSocketCount;
};

#endif // !__CONNECT_H__


