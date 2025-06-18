#pragma once
#include "PacketProtocol.hpp"

#include "MfrConfig.h"

class StepMotorController {
private:
    int uart_fd = -1;

    MfrConfig mfrConfig;

    std::string device;
    speed_t  uartBaudRate;    

public:
    StepMotorController();
    ~StepMotorController();
    void sendCommand(const std::string& cmd);

private:    
    bool initUart();
};