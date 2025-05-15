cmake_minimum_required(VERSION 3.10)
project(SurfaceToAirWeaponSystem)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/Simulator
    ${CMAKE_SOURCE_DIR}/UDPCommunicate
    ${CMAKE_SOURCE_DIR}/Mock
    ${CMAKE_SOURCE_DIR}/Mock/info
)

# Add source files
set(SOURCES
    main.cpp
    Simulator/Simulator.cpp
    UDPCommunicate/LSRecvUDPManager.cpp
    UDPCommunicate/MFRSendUDPManager.cpp
    Mock/MockTarget.cpp
    Mock/MockTargetManager.cpp
    Mock/MockMissileManager.cpp
    Mock/MockMissile.cpp
)

# Add header files
set(HEADERS
    Simulator/Simulator.h
    UDPCommunicate/LSRecvUDPManager.h
    UDPCommunicate/MFRSendUDPManager.h
    Mock/MockTargetManager.h
    Mock/MockTarget.h
    Mock/MockMissileManager.h
    Mock/info/MissileInfo.h
)

# Create the executable
add_executable(SurfaceToAirWeaponSystem ${SOURCES} ${HEADERS})

# Add threading support
find_package(Threads REQUIRED)
target_link_libraries(SurfaceToAirWeaponSystem Threads::Threads)