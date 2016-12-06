#include "multi_process_server.h"

int main(int argc, char* argv[])
{
	do {
		multi_process_server::getInstance()->run("0.0.0.0", 10800);
	} while(0);
}
