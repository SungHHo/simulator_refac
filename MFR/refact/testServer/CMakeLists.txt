cmake_minimum_required(VERSION 3.10)
project(TestServer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# protocol 헤더 위치 (MFR/refact 기준에서 ../../protocol)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../protocol
)

add_executable(TestServer
    TestServer.cpp
)
