//
// Created by tanzilya on 05.05.2021.
//

#ifndef BOOST_PROXYSERVER_H
#define BOOST_PROXYSERVER_H
#include <utility>

#include "../DbConnector/DbConnector.h"

//void ProxyServer::startSession() {
//    startAccept();
//}
//
//void handleAccept(ProxyServer *proxy, const boost::system::error_code &err) {
//    if (err) return;
//    boost::system::error_code error;
//    char data[4096];
////    size_t ret = (*proxy->_socket).read_some(boost::asio::buffer(data), error);
////    data[ret] = '\0';
////    std::cout << YELLOW << ret << DEFAULT << std::endl;
////    std::cout << BLUE << data << DEFAULT << std::endl;
////    std::string h = "127.0.0.1"; int p = 5432;
//    DbConnector connector(proxy->_ios, "127.0.0.1", 5432, proxy->_ep,
//                          reinterpret_cast<boost::asio::ip::tcp::socket *>(&(proxy->_socket)));
//    connector.connect();
//    socketPtr tmp(new boost::asio::ip::tcp::socket(proxy->_ios));
//    proxy->_socket = tmp;
//    proxy->startAccept();
//}
//
//void ProxyServer::startAccept() {  //add new client connections
//    _acc.async_accept(*_socket, boost::bind(handleAccept, this, _1));
//}

class ProxyServer {
private:
    boost::asio::io_service& _ios;
    boost::asio::ip::tcp::acceptor _acc;
    DbConnector * _session;
    int _dbPort;
    std::string _dbHost;

public:
    ProxyServer(boost::asio::io_service& ios, const std::string host, int port, std::string  dbHost, int dbPort)
                    : _ios(ios),
                      _acc(_ios, boost::asio::ip::tcp::endpoint(
                              boost::asio::ip::address_v4::from_string(host), port)),
                      _dbPort(dbPort),
                      _dbHost(std::move(dbHost)) {}

    void acceptConnections() {
        _session = new DbConnector(_ios);
        _acc.async_accept(_session->getClientSock(),
                          boost::bind(&ProxyServer::startRequestProcessing,this,
                                 boost::asio::placeholders::error));
    }

private:

    void startRequestProcessing(const boost::system::error_code& error) {
        if (error) {
            std::cout << error.message() << std::endl;
            return;
        }
        _session->connect(_dbHost, _dbPort);
       try { acceptConnections(); }
       catch (std::exception & ex) {
           std::cerr << ex.what() << std::endl;
       }
    }
};


#endif //BOOST_PROXYSERVER_H
