#pragma once

#include <string>
#include <vector>
#include <thread>
#include "SerialReceiverInterface.hpp"
#include "info.hpp"
class LCToLSCommManager
{
private:
    int fd = -1;
    std::string serialPath;
    SerialReceiverInterface& receiver;
    std::thread listenerThread;

    void init(const std::string& configPath);
    int configureUART(const std::string& devicePath, int baudRate, int dataBits, int stopBits, char parity);
    void run();

public:
    LCToLSCommManager(SerialReceiverInterface& receiver, const std::string& configPath);
    ~LCToLSCommManager();
    void sendData(const std::vector<uint8_t>& packet);
};