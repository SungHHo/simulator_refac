#pragma once

#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <chrono>

#include "PacketProtocol.hpp"
#include "MfrConfig.h"

class StepMotorController {
private:
    /// @brief UART fd
    int uart_fd = -1;
    /// @brief Socket fd
    int sock_fd = -1;
    /// 설정 INI
    MfrConfig mfrConfig;
    /// @brief UART 장치
    std::string device;
    /// @brief UART Baud Rate
    speed_t  uartBaudRate;
    /// @brief Socket 서버 IP
    std::string serverIp;
    /// @brief Socket 서버 Port
    int serverPort;

public:
    /// @brief 생성자
    StepMotorController();
    /// @brief 소멸자
    ~StepMotorController();
    /// @brief 
    /// @param cmd 
    void sendCommand(const std::string& cmd);
    /// @brief 
    /// @param cmd 
    void sendCmd(const std::string& cmd);

private:
    /// @brief UART 초기화    
    bool initUart();
    /// @brief 
    /// @return 
    void connectToServer();
};
