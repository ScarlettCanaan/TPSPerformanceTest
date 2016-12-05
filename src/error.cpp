#include "error.h"

void error::PRINT_ERROR(const char *msg, const int r)
{
	do {
		std::cerr << msg << ": [ " << uv_err_name((r)) << " : " << uv_strerror((r)) << " ]"<< std::endl;
		assert(0);
	} while (0);
}

error::error()
{
}


error::~error()
{
}
