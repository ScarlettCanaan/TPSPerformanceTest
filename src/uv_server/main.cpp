#include <stdlib.h>
#include "server.h"
#include "multi_process_server.h"

static long port;

void Usage() {
    std::cout << "Usage: server port [[ -t | -p ] [count]]" << std::endl;
    std::cout << "      port:   server establish to listen which [port]" << std::endl;
    std::cout << "      -t :    server run with multi threading mode. Using [count] to specify thread count." << std::endl;
    std::cout << "      -p :    server run with multi process mode. Using [count] to specify process count." << std::endl;
}

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
    if (argc == 2 || argv[1] == "-h") {
        Usage();
        return 0;
    }

    if (argc < 2 || argc > 4) {
        Usage();
        return -1;
    }  
    port = strtol(argv[1], NULL, 10);
    if (ERANGE == port) {
        Usage();
        return -1;
    }
    if (argc > 2) {
        switch (parse(argv[2])) {
            case 1:
                if (argc == 4) {
                    int count = strtol(argv[3], NULL, 10);
                    if (ERANGE == count) {
                        Usage();
                        return -1;
                    } else {
                        do {
                            server::getInstance()->run("0.0.0.0", port);
                        } while(0);
                    }
                } else {
                    do {
                        server::getInstance()->run("0.0.0.0", port);
                    } while(0);
                }
                break;
            case 2:
                if (argc == 4) {
                    int count = strtol(argv[3], NULL, 10);
                    if (ERANGE == count) {
                        Usage();
                        return -1;
                    } else {
                        do {
                            multi_process_server::getInstance()->run("0.0.0.0", port);
                        } while(0);
                    }
                } else {
                    do {
                        multi_process_server::getInstance()->run("0.0.0.0", port);
                    } while(0);
                }
                break;
            case 0:
            default:
                Usage();
                return -1;
        }
    }
}
