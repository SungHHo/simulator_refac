cmake_minimum_required(VERSION 3.10)
project(TempLc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lc_server main.cpp)

target_link_libraries(lc_server pthread)
