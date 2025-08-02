cmake_minimum_required(VERSION 3.10)
project(Mfr)

set(PROTOCOL_LIB_PATH /usr/local/lib)
set(PROTOCOL_INCLUDE_PATH /usr/local/include)

include_directories(
    ${PROTOCOL_INCLUDE_PATH}
    ../../protocol 
    communication
    config
    parsing
)

link_directories(${PROTOCOL_LIB_PATH})

add_executable(MfrTest
    Mfr.cpp
    communication/MfrLcCommunicationManager.cpp
    config/MfrConfig.cpp
    parsing/MfrParsing.cpp
)

target_link_libraries(MfrTest protocol)
