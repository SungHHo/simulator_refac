cmake_minimum_required(VERSION 3.10)
project(Mfr)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ../../protocol
    communication
    config
    parsing
)

add_executable(MfrTest
    Mfr.cpp
    communication/MfrLcCommunicationManager.cpp
    config/MfrConfig.cpp
    parsing/MfrParsing.cpp
)
