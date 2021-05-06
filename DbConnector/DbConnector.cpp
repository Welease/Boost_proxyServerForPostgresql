//
// Created by tanzilya on 05.05.2021.
//

#include "DbConnector.h"
#include "../Server/ProxyServer.h"

DbConnector::DbConnector(boost::asio::io_service& ios, int logFd) : _clientSock(ios),
                                                                    _dbSock (ios),
                                                                    _numOfRequest(0),
                                                                    _logFd(logFd){
    fillTypesMap();
}

void DbConnector::connect(const std::string& dbHost, int dbPort) {
    _dbSock.async_connect(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string(dbHost), dbPort),
                          boost::bind(&DbConnector::startDataExchange, this,
                                  boost::asio::placeholders::error));
}

void DbConnector::startDataExchange(const boost::system::error_code& error) {
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

void DbConnector::recieveRequest(const boost::system::error_code& error) {
    if (errorOccurred(error))
        return;
    _clientSock.async_read_some(boost::asio::buffer(_request, bufSize),
                                boost::bind(&DbConnector::sendRequest, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
}

void DbConnector::sendRequest(const boost::system::error_code& error, const size_t& bytes_transferred) {
    if (errorOccurred(error))
        return;
    _numOfRequest++;
    _bytes = bytes_transferred;
    makeLog();
    async_write(_dbSock, boost::asio::buffer(_request, bytes_transferred),
                boost::bind(&DbConnector::recieveRequest, this,
                            boost::asio::placeholders::error));
}

void DbConnector::recieveResponse(const boost::system::error_code& error) {
    if (errorOccurred(error))
        return;
    _dbSock.async_read_some(boost::asio::buffer(_response, bufSize),
                            boost::bind(&DbConnector::sendResponse, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void DbConnector::sendResponse(const boost::system::error_code& error, const size_t& bytes_transferred) {
    if (errorOccurred(error))
        return;
    async_write(_clientSock, boost::asio::buffer(_response, bytes_transferred),
                boost::bind(&DbConnector::recieveResponse, this,
                        boost::asio::placeholders::error));
}

bool DbConnector::errorOccurred(const boost::system::error_code& error) {
    if (!error)
        return false;
    endOfSession();
    return true;
}

void DbConnector::endOfSession() {
    _clientSock.close();
    _dbSock.close();
}

boost::asio::ip::tcp::socket & DbConnector::getClientSock() {
    return _clientSock;
}

int DbConnector::getIntFromHex(unsigned char *hex) {
    return (int(hex[0]) * int(std::pow(16, 4)) + int(hex[1]) * int(std::pow(16, 3)) +
            int(hex[2]) * int(std::pow(16, 2)) + int(hex[3]));
}

void DbConnector::writeInfo(const char *header, size_t len1, const char *message, size_t len) {
    write(_logFd, header, len1);
    for (size_t i = 0; i < len; ++i)
        if (message[i] != '\0') write(_logFd, &message[i], 1);
    write(_logFd, "\n", 1);
}

void DbConnector::makeLog() {
    char		timeBuff[100];

    time_t time_ = time(nullptr);
    struct tm * local = localtime(&time_);
    size_t timeLen = strftime(timeBuff, 80, "%F %X ", local);
    _packetLength = std::to_string(getIntFromHex(_numOfRequest == 1 ? _request : (_request + 1)));
    auto key = messageTypes.find(_request[0]);
    if (key != messageTypes.end())
        _protocol = key->second;
    if (_numOfRequest == 1) {
        _protocol = messageTypes.find('-')->second;
    }
    writeInfo("DATE: ", 6, timeBuff, timeLen);
    writeInfo("PAYLOAD LENGTH: ", 16, _packetLength.c_str(), _packetLength.length());
    writeInfo("MESSAGE TYPE: ", 14, _protocol.c_str(), _protocol.length());
    for (size_t i = _numOfRequest == 1 ? 8 : 5; i <  _bytes - 1; ++i) {
        write(_logFd, &_request[i], 1);
        if (_request[i] == ';' && i != _bytes - 1) write(_logFd, "\n", 1);
    }
    write(_logFd, "\n------------------------------------------------\n", 50);
}

void DbConnector::fillTypesMap() {
    messageTypes.insert(std::pair<unsigned char, std::string>('B', "Bind"));
    messageTypes.insert(std::pair<unsigned char, std::string>('C', "Close"));
    messageTypes.insert(std::pair<unsigned char, std::string>('d', "CopyData"));
    messageTypes.insert(std::pair<unsigned char, std::string>('c', "CopyDone"));
    messageTypes.insert(std::pair<unsigned char, std::string>('f', "CopyFail"));
    messageTypes.insert(std::pair<unsigned char, std::string>('E', "Execute"));
    messageTypes.insert(std::pair<unsigned char, std::string>('H', "Flush"));
    messageTypes.insert(std::pair<unsigned char, std::string>('P', "Parse"));
    messageTypes.insert(std::pair<unsigned char, std::string>('D', "Describe"));
    messageTypes.insert(std::pair<unsigned char, std::string>('F', "FunctionCall"));
    messageTypes.insert(std::pair<unsigned char, std::string>('Q', "Query"));
    messageTypes.insert(std::pair<unsigned char, std::string>('-', "StartupMessage"));
    messageTypes.insert(std::pair<unsigned char, std::string>('p', "PasswordMessage"));
    messageTypes.insert(std::pair<unsigned char, std::string>('S', "Sync"));
    messageTypes.insert(std::pair<unsigned char, std::string>('X', "Terminate"));
}