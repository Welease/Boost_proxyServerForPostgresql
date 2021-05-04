//
// Created by tanzilya on 04.05.2021.
//

#ifndef BOOST_DBCONNECTER_H
#define BOOST_DBCONNECTER_H


#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class DbConnector {
private:
    boost::asio::io_service    &        _ios;
    boost::asio::ip::tcp::endpoint      _dbEp;
    boost::asio::ip::tcp::endpoint      _clientEp;
    boost::asio::ip::tcp::socket        _dbSocket;
    boost::asio::ip::tcp::socket    *   _clientSocket;
    const static size_t                 _bufSize = 4096;
    char                         _request[_bufSize];
    char                         _response[_bufSize];

public:
    DbConnector(boost::asio::io_service & ios, std::string host, int port,
                boost::asio::ip::tcp::endpoint & clientEp,
                boost::asio::ip::tcp::socket * clSock);
    ~DbConnector();
    void connect();
    void connectToDb(const boost::system::error_code & ec);
    void sendResponse(const boost::system::error_code& error, const size_t& bytes_transferred);
    void recieveRequest(const boost::system::error_code& error);
    void sendRequest(const boost::system::error_code& error, const size_t& bytes_transferred);
    void recieveResponse(const boost::system::error_code& error);

};


#endif //BOOST_DBCONNECTER_H
