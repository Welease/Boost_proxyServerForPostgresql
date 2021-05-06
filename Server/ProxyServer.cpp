//
// Created by tanzilya on 05.05.2021.
//

#include "ProxyServer.h"

ProxyServer::ProxyServer(boost::asio::io_service &ios,  std::string host, int port, std::string dbHost, int dbPort, int fd) :
                                                        _ios(ios),
                                                        _acc(_ios, boost::asio::ip::tcp::endpoint(
                                                        boost::asio::ip::address_v4::from_string(host), port)),
                                                        _dbPort(dbPort), _dbHost(std::move(dbHost)),
                                                        _session(nullptr),
                                                        _fdLog(fd){
    std::cout << GREEN << "ProxyServer started working" << DEFAULT << std::endl;
    signal(SIGPIPE, SIG_IGN);
}

ProxyServer::~ProxyServer() {
    close(_fdLog);
}

void ProxyServer::acceptConnections() {
    _session = new DbConnector(_ios, _fdLog);
    _acc.async_accept(_session->getClientSock(),
                      boost::bind(&ProxyServer::startRequestProcessing,this,
                              boost::asio::placeholders::error));
}

void ProxyServer::startRequestProcessing(const boost::system::error_code& error) {
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