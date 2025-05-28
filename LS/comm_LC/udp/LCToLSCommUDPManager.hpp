#pragma once

#include <string>
#include <vector>
#include <thread>
#include <netinet/in.h>
#include "SerialReceiverInterface.hpp"

class LCToLSUDPCommManager
{
private:
    int recvSock = -1;
    int sendSock = -1;
    std::string remoteIp;
    int remotePort = 0;
    std::thread listenerThread;
    sockaddr_in remoteAddr{}, localAddr{};
    SerialReceiverInterface& receiver;

    void run();

public:
    LCToLSUDPCommManager(SerialReceiverInterface& receiver, const std::string& configPath);
    ~LCToLSUDPCommManager();

    void init(const std::string& configPath);
    void sendData(const std::vector<uint8_t>& packet);
};
