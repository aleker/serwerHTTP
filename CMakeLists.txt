cmake_minimum_required(VERSION 3.6)
project(serwerHTTP)

# output directory:
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# 1
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lconfig++ -Wall -pthread")

add_executable(serverHTTP
        serwerHTTP.cpp
        Record.cpp
        StreamRecord.cpp
        BeginRecord.cpp
        ConnectionManager.cpp
        Parser.cpp
        HTTPManager.cpp
        FCGIManager.cpp
        ConfigFile.cpp)

# 2
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lfcgi++ -lfcgi -lconfig++ -Wall -pthread")

add_executable(fcgi fcgi.cpp ConfigFile.cpp)
