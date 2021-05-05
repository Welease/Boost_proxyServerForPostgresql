cmake_minimum_required(VERSION 3.17)
project(boost)


set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-pthread")

add_executable(ProxyServer Server/ProxyServer.cpp Server/ProxyServer.h DbConnector/DbConnector.cpp DbConnector/DbConnector.h main.cpp)