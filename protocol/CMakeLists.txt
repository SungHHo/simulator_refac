cmake_minimum_required(VERSION 3.5)
project(protocol)

add_library(protocol STATIC
    ByteBuffer.cpp
    Header1.cpp
    Header2.cpp
    Payload2.cpp
    Packet.cpp
)

# 빌드 시 내부에서만 사용되는 헤더 경로
target_include_directories(protocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 라이브러리 설치
install(TARGETS protocol DESTINATION lib)

# (확장자 없는 protocol 헤더)
install(FILES protocol DESTINATION include)
