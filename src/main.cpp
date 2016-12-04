#include "server.h"
int main(int argc, char* argv[])
{
	do
	{
		server::getInstance()->run("0.0.0.0", 10800);
	} while (0);
}
