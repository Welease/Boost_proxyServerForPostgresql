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
    DbConnector(boost::asio::io_service& ios) : _clientSock(ios), _dbSock (ios) {}

    void connect(const std::string& dbHost, int dbPort) {
        _dbSock.async_connect(boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address::from_string(dbHost), dbPort),
                boost::bind(&DbConnector::startDataExchange, this,
                boost::asio::placeholders::error));
    }

    void startDataExchange(const boost::system::error_code& error) {
        if (errorOccurred(error))
            return;
        _dbSock.async_read_some(boost::asio::buffer(_response, bufSize),
                                boost::bind(&DbConnector::sendResponse, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
        _clientSock.async_read_some(boost::asio::buffer(_request, bufSize),
                                boost::bind(&DbConnector::sendRequest, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }

    boost::asio::ip::tcp::socket & getClientSock() {
        return _clientSock;
    }

private: //methods
    void sendResponse(const boost::system::error_code& error, const size_t& bytes_transferred) {
            if (errorOccurred(error))
                return;
            async_write(_clientSock, boost::asio::buffer(_response, bytes_transferred),
                        boost::bind(&DbConnector::recieveResponse, this,
                                    boost::asio::placeholders::error));
        }

        void recieveResponse(const boost::system::error_code& error) {
            if (errorOccurred(error))
                return;
            _dbSock.async_read_some(boost::asio::buffer(_response, bufSize),
                                    boost::bind(&DbConnector::sendResponse, this,
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred));
        }

        void sendRequest(const boost::system::error_code& error, const size_t& bytes_transferred) {
            if (errorOccurred(error))
                return;
            async_write(_dbSock, boost::asio::buffer(_request, bytes_transferred),
                        boost::bind(&DbConnector::recieveRequest, this,
                                    boost::asio::placeholders::error));
        }

        void recieveRequest(const boost::system::error_code& error) {
            if (errorOccurred(error))
                return;
            _clientSock.async_read_some(boost::asio::buffer(_request, bufSize),
                                        boost::bind(&DbConnector::sendRequest, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
        }

        bool errorOccurred(const boost::system::error_code& error) {
            if (!error)
                return false;
            endOfSession();
            return true;
        }

        void endOfSession() {
            _clientSock.close();
            _dbSock.close();
        }
};

#endif //BOOST_DBCONNECTOR_H
