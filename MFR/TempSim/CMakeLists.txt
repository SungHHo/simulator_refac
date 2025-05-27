cmake_minimum_required(VERSION 3.10)
project(TempSimUDP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tempsim
    main.cpp
)

target_link_libraries(tempsim pthread)
