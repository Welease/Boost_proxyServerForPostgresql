#include <iostream>
#include "DbConnector/DbConnector.h"
#include "Server/ProxyServer.h"

int main(int argc, char **argv) {
    if (argc == 6) {
        boost::asio::io_service ios;
        std::string proxyIp = argv[1];
        int proxyPort = atoi(argv[2]);
        std::string dbIp = argv[3];
        int dbPort = atoi(argv[4]);
        int fdLog = open(argv[5], O_CREAT | O_RDWR | 0666);
        if (fdLog < 0) {
            std::cout << RED << "Incorrect file from logs" << DEFAULT << std::endl;
            return 0;
        }
        try {
            ProxyServer proxyServer(ios, proxyIp, proxyPort, dbIp, dbPort, fdLog);
            proxyServer.acceptConnections();
            ios.run();
        } catch (std::exception & ex) {
            std::cout << RED << ex.what()  << DEFAULT << std::endl;
        }
    } else {
        std::cout << RED << "Usage: ./proxy <proxy server ip> <proxy server port> "
                            "<database host ip> <database port> <file for logs>" << DEFAULT << std::endl;
    }
}
