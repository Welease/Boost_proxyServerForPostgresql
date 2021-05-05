#include <iostream>
#include "DbConnector/DbConnector.h"
#include "Server/ProxyServer.h"

int main(int argc, char* argv[]) {

    boost::asio::io_service ios;

    try {
        ProxyServer acceptor(ios, "127.0.0.1", 8080, "127.0.0.1", 5432);

        acceptor.acceptConnections();

        ios.run();
    }
    catch(std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
