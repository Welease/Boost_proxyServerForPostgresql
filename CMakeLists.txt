cmake_minimum_required(VERSION 3.17)
project(boost)

set(CMAKE_CXX_STANDARD 14)

#set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
find_package(Boost 1.42.0 REQUIRED COMPONENTS thread)
IF(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

add_executable(ProxyServer Server/ProxyServer.cpp Server/ProxyServer.h DbConnector/DbConnector.cpp DbConnector/DbConnector.h main.cpp)

target_link_libraries(ProxyServer ${Boost_THREAD_LIBRARY})