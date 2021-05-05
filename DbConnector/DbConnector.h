//
// Created by tanzilya on 05.05.2021.
//

#ifndef BOOST_DBCONNECTOR_H
#define BOOST_DBCONNECTOR_H

#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

class DbConnector {

private: //parameters
    boost::asio::ip::tcp::socket _clientSock;
    boost::asio::ip::tcp::socket _dbSock;

    const static int bufSize = 4096;
    unsigned char _request[bufSize];
    unsigned char _response[bufSize];

public:
    DbConnector(boost::asio::io_service& ios);
    void connect(const std::string& dbHost, int dbPort);
    void startDataExchange(const boost::system::error_code& error);
    boost::asio::ip::tcp::socket & getClientSock();

private: //methods
    void recieveRequest(const boost::system::error_code& error);
    void sendRequest(const boost::system::error_code& error, const size_t& bytes_transferred);
    void recieveResponse(const boost::system::error_code& error);
    void sendResponse(const boost::system::error_code& error, const size_t& bytes_transferred);
    bool errorOccurred(const boost::system::error_code& error);
    void endOfSession();
};

#endif //BOOST_DBCONNECTOR_H
