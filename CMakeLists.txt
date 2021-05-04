cmake_minimum_required(VERSION 3.17)
project(boost)


set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-pthread")
#set (CMAKE_CXX_FLAGS "-pedantic -ansi -Wall -Werror -O3 -L/usr/lib -lstdc++ -lpthread -lboost_thread -lboost_system")

add_executable( boost
                main.cpp
                ProxyServer/ProxyServer.cpp
                ProxyServer/ProxyServer.h
                DbConnector/DbConnector.cpp
                DbConnector/DbConnector.h)
