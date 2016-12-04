#include "error.h"

void error::PRINT_ERROR(const char *msg, const int r)
{
	do {
		std::cerr << msg << ": " << strerror(errno) << std::endl;
		assert(0);
	} while (0);
}

error::error()
{
}


error::~error()
{
}
