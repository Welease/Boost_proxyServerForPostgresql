#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

using namespace boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socketPtr;
void start_accept(socketPtr &sock);
void handle_accept(socketPtr &sock, const boost::system::error_code & err);
io_service service;
ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 2001);
ip::tcp::acceptor acc(service, ep);

void start_accept(socketPtr& sock) {
    acc.async_accept(*sock, boost::bind( handle_accept, sock, _1) ); //boost:bind
}

void handle_accept(socketPtr &sock, const boost::system::error_code & err) {
    if (err) return;
    char data[1024];
    boost::system::error_code error;
    size_t length = sock->read_some(buffer(data), error);
    std::cout << length << std::endl;
    std::cout << data << std::endl;
    socketPtr newSocket(new ip::tcp::socket(service));
    sock = newSocket;
    start_accept(sock);
}

int main() {
    socketPtr sock(new ip::tcp::socket(service));
    start_accept(sock);
    service.run();
}
