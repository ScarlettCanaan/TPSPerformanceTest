#include <stdlib.h>
#include <gflags/gflags.h>
#include "server.h"
#include "multi_process_server.h"

#ifdef WIN32
#pragma comment(lib, "libuv.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Psapi.lib")
#include <Winsock2.h>
#endif // WIN32

static const std::string usageMsg = {
	"Usage : server required_port_input [ options ].\r\nOptions:\r\n\t--multithread\t\tserver run with multi threading mode. \r\n\t--multiprocess\t\tserver run with multi process mode."
};
static const std::string versionMsg = {
	"Version:\tv0.1.2"
};
DEFINE_bool(multithread, false, "runing with multithreading mode.");
DEFINE_bool(multitprocess, false, "runing with multiprocess mode.");
DEFINE_int32(port, 10800, "server establish to listen which [port].");
static long port = 10800;

int parse(char *ch) {
    if (ch == "-t") {
        return 1;
    }
    if (ch == "-p") {
        return 2;
    }
    return 0;
}

int main(int argc, char* argv[])
{
	google::SetUsageMessage(usageMsg);
	google::SetVersionString(versionMsg);
	google::ParseCommandLineFlags(&argc, &argv, true);
	port = FLAGS_port;
	if (FLAGS_multithread && !FLAGS_multitprocess) {
		do {
			server::getInstance()->run("0.0.0.0", port);
		} while (0);
	}
	else if (FLAGS_multitprocess && !FLAGS_multithread) {
		do {
			multi_process_server::getInstance()->run("0.0.0.0", port);
		} while (0);
	}
	else {
		google::ShowUsageWithFlags(0);
    }
}
