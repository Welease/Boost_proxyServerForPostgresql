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
    boost::asio::ip::tcp::socket  _clientSock;
    boost::asio::ip::tcp::socket  _dbSock;
    int                           _logFd;
    int                           _numOfRequest;
    std::string                   _protocol;
    std::string                   _packetLength;
    size_t                        _bytes;

    const static int bufSize = 4096;
    unsigned char _request[bufSize];
    unsigned char _response[bufSize];
    std::map<unsigned char, std::string> messageTypes;

public:
    DbConnector(boost::asio::io_service& ios, int logFd);
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
    int  getIntFromHex(unsigned char *hex);
    void writeInfo(const char *header, size_t len1, const char *message, size_t len);
    void makeLog();
    void fillTypesMap();
};

#endif //BOOST_DBCONNECTOR_H
