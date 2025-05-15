cmake_minimum_required(VERSION 3.10)
project(UDPTestPrograms)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add source files for testclient
set(TESTCLIENT_SOURCES
    LSRecvUDPManager.cpp
)

# Add source files for mfr_listener
set(MFR_LISTENER_SOURCES
    MFRSendUDPManager.cpp
)

# Create the executable for the test client
add_executable(testclient ${TESTCLIENT_SOURCES})

# Create the executable for the MFR listener
add_executable(mfr_listener ${MFR_LISTENER_SOURCES})

# Add necessary libraries (if needed)
target_link_libraries(testclient PRIVATE pthread)
target_link_libraries(mfr_listener PRIVATE pthread)