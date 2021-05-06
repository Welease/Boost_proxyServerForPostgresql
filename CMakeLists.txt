cmake_minimum_required(VERSION 3.16)
project(boost)

set(CMAKE_CXX_STANDARD 14)

set(Boost_USE_MULTITHREAD ON)
find_package(Boost 1.42.0 REQUIRED COMPONENTS thread)
IF(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

add_executable(proxy Server/ProxyServer.cpp Server/ProxyServer.h DbConnector/DbConnector.cpp DbConnector/DbConnector.h main.cpp)

target_link_libraries(proxy ${Boost_THREAD_LIBRARY})