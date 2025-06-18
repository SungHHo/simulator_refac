cmake_minimum_required(VERSION 3.10)
project(RadarSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)  # 기본 빌드 타입을 Debug로 설정
set(CMAKE_CXX_FLAGS_DEBUG "-g")  # -g 플래그로 디버깅 정보 포함

# 소스 파일들
set(SOURCES
    main.cpp
    Mfr.cpp
    MfrLcCommManager.cpp
    MfrSimCommManager.cpp
    StepMotorController.cpp
    config/MfrConfig.cpp
)

# 실행 파일 생성
add_executable(radar ${SOURCES})

# include 디렉토리 추가
target_include_directories(radar PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/config
)

# pthread 사용 시 연결 (sleep, thread 등)
target_link_libraries(radar pthread)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/MFR.ini
    ${CMAKE_CURRENT_BINARY_DIR}/config/MFR.ini
    COPYONLY
)