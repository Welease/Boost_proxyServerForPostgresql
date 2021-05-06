//
// Created by tanzilya on 05.05.2021.
//

#ifndef BOOST_PROXYSERVER_H
#define BOOST_PROXYSERVER_H

#include "../DbConnector/DbConnector.h"
# define DEFAULT "\e[39m\e[0m"
# define GREEN "\e[92m"
# define RED  "\e[31m"
# define YELLOW "\e[1;33m"
# define BLUE "\e[1;34m"
# define BUF_SIZE 1000000

class ProxyServer {

private:
    int                             _dbPort;
    std::string                     _dbHost;
    boost::asio::io_service&        _ios;
    boost::asio::ip::tcp::acceptor  _acc;
    DbConnector *                   _session;
    int                             _fdLog;

public:
    ProxyServer(boost::asio::io_service& ios, std::string host, int port, std::string  dbHost, int dbPort, int fd);
    ~ProxyServer();
    void acceptConnections();
    void startRequestProcessing(const boost::system::error_code& error);
};


#endif //BOOST_PROXYSERVER_H
