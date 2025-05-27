#pragma once
#include "IReceiver.hpp"
#include <vector>

class MfrSimCommManager {
private:
    IReceiver* receiver;

public:
    MfrSimCommManager(IReceiver* recv = nullptr) : receiver(recv) {}    
    void startUdpReceiver();
    void recv(const std::vector<char>& packet);
    void setReceiver(IReceiver* recv);
};