cmake_minimum_required(VERSION 3.14)
project(LauncherSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 소스 모음
set(SOURCES
    main.cpp
    LS.cpp
    config/ConfigParser.cpp
    comm_SIM/LSToSimCommManager.cpp
    # comm_LC/LCToLSCommManager.cpp
    comm_LC/udp/LCToLSCommUDPManager.cpp
    statusManager/LSStatusManager.cpp
    statusManager/motorManager/LSMotorManager.cpp

)

# 실행 파일 생성
add_executable(launcher ${SOURCES})

# include 경로 설정
target_include_directories(launcher PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/config
    ${PROJECT_SOURCE_DIR}/comm_SIM
    ${PROJECT_SOURCE_DIR}/comm_LC
    ${PROJECT_SOURCE_DIR}/comm_LC/udp
    ${PROJECT_SOURCE_DIR}/statusManager
    ${PROJECT_SOURCE_DIR}/statusManager/motorManager
)
