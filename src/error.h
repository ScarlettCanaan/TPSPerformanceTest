#ifndef __ERROR_H__
#define __ERROR_H__

#include <assert.h>
#include <iostream>
#include <errno.h>
#include <string.h>
class error
{
public:
	static void PRINT_ERROR(const char *msg, const int r);

public:
	error();
	~error();
};

#endif // !__ERROR_H__

