//
// Created by tanzilya on 04.05.2021.
//

#include "ProxyServer.h"

ProxyServer::ProxyServer(std::string &host,
                         int &port, boost::asio::io_service & ios, int & fd) :
                         _host(host),
                         _port(port),
                         _ios(ios),
                         _logFd(fd),
                         _ep(boost::asio::ip::address::from_string(host), _port),
                         _acc(ios, _ep),
                         _socket(new boost::asio::ip::tcp::socket(_ios)){}

ProxyServer::ProxyServer(ProxyServer &proxyServer) :
        _host(proxyServer._host),
        _port(proxyServer._port),
        _ios(proxyServer._ios),
        _logFd(proxyServer._logFd),
        _ep(boost::asio::ip::address::from_string(_host), _port),
        _acc(_ios, _ep),
        _socket(new boost::asio::ip::tcp::socket(_ios)) {}

ProxyServer::~ProxyServer() {
    close(_logFd);
}

void ProxyServer::startSession() {
    startAccept();
}

void handleAccept(ProxyServer *proxy, const boost::system::error_code &err) {
    if (err) return;
    boost::system::error_code error;
    char data[4096];
//    size_t ret = (*proxy->_socket).read_some(boost::asio::buffer(data), error);
//    data[ret] = '\0';
//    std::cout << YELLOW << ret << DEFAULT << std::endl;
//    std::cout << BLUE << data << DEFAULT << std::endl;
//    std::string h = "127.0.0.1"; int p = 5432;
    DbConnector connector(proxy->_ios, "127.0.0.1", 5432, proxy->_ep,
                          reinterpret_cast<boost::asio::ip::tcp::socket *>(&(proxy->_socket)));
    connector.connect();
    socketPtr tmp(new boost::asio::ip::tcp::socket(proxy->_ios));
    proxy->_socket = tmp;
    proxy->startAccept();
}

void ProxyServer::startAccept() {  //add new client connections
    _acc.async_accept(*_socket, boost::bind(handleAccept, this, _1));
}

