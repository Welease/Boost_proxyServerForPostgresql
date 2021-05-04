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
typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socketPtr;

class ProxyServer {
private:
    std::string                 _host;
    int                         _port;
    boost::asio::io_service     _ios;
    char *                      _data;

public:
    ProxyServer(std::string & host, int & port);
    ~ProxyServer();

    void startSession();
};


#endif //BOOST_PROXYSERVER_H
