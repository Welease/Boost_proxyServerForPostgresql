//
// Created by tanzilya on 04.05.2021.
//

#ifndef BOOST_PROXYSERVER_H
#define BOOST_PROXYSERVER_H

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include "../DbConnector/DbConnector.h"
# define DEFAULT "\e[39m\e[0m"
# define GREEN "\e[92m"
# define RED  "\e[31m"
# define YELLOW "\e[1;33m"
# define BLUE "\e[1;34m"

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socketPtr;

class ProxyServer {
private:                                    //parameters
    std::string                     _host;
    int                             _port;
    boost::asio::io_service   &     _ios;
    boost::asio::ip::tcp::endpoint  _ep;
    boost::asio::ip::tcp::acceptor  _acc;
    socketPtr                       _socket;
    const static int                _bufSize = 4096;
    static char                     _data[_bufSize];
    int                             _logFd;

public:
    ProxyServer(std::string & host, int & port, boost::asio::io_service & ios, int & fd);
    ProxyServer(ProxyServer & proxyServer);
    ~ProxyServer();

    void startSession();

private:                                    //methods
    void startAccept();
    friend void handleAccept(ProxyServer * proxy, const boost::system::error_code &err);
};

void handleAccept(ProxyServer * proxy, const boost::system::error_code &err);


#endif //BOOST_PROXYSERVER_H
