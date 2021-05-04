//
// Created by tanzilya on 04.05.2021.
//
#include "DbConnector.h"

DbConnector::DbConnector(boost::asio::io_service & ios, std::string host, int port,
                        boost::asio::ip::tcp::endpoint & clientEp,
                        boost::asio::ip::tcp::socket * clSock) :
        _ios(ios),
        _dbEp(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host), port)),
        _clientEp(clientEp),
        _dbSocket(boost::asio::ip::tcp::socket(_ios)),
        _clientSocket(clSock) { }

DbConnector::~DbConnector() {}

void DbConnector::connect() {
    _dbSocket.async_connect(_dbEp, boost::bind(&DbConnector::connectToDb, this,
                        boost::asio::placeholders::error));
}

void DbConnector::connectToDb(const boost::system::error_code &ec) {
    if (ec) return;
    _dbSocket.async_read_some(boost::asio::buffer(_response, _bufSize),
                              boost::bind(&DbConnector::sendResponse, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
        _clientSocket->async_read_some(
                boost::asio::buffer(_request, _bufSize),
                boost::bind(&DbConnector::sendRequest, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void DbConnector::sendResponse(const boost::system::error_code &error, const size_t &bytes_transferred) {
    if (error) return;
    async_write(*_clientSocket, boost::asio::buffer(_response,bytes_transferred),
                    boost::bind(&DbConnector::recieveRequest, this,
                                boost::asio::placeholders::error));
}


void DbConnector::recieveResponse(const boost::system::error_code &error) {
    if (error) return;
        _dbSocket.async_read_some(
                boost::asio::buffer(_response,_bufSize),
                boost::bind(&DbConnector::sendResponse, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void DbConnector::sendRequest(const boost::system::error_code& error, const size_t& bytes_transferred) {
    if (error) return;
    async_write(_dbSocket, boost::asio::buffer(_request, bytes_transferred),
                boost::bind(&DbConnector::recieveResponse, this,
                                boost::asio::placeholders::error));
}


void DbConnector::recieveRequest(const boost::system::error_code &error) {
    if (error) return;
        _clientSocket->async_read_some(
                boost::asio::buffer(_request, _bufSize),
                boost::bind(&DbConnector::sendRequest,
                            this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}
