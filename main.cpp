#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include "ProxyServer/ProxyServer.h"

using namespace boost::asio;
io_service service;

int main() {
    std::string h = "127.0.0.1";
    int p = 8080;
    int fd = 1;
    ProxyServer proxyServer(h, p, service, fd);
    proxyServer.startSession();
    service.run();
}
