#pragma once
#include "IReceiver.hpp"
#include <vector>

class MfrLcCommManager {
private:
    int sockfd;
    IReceiver* receiver;

public:
    MfrLcCommManager(IReceiver* receiver = nullptr);
    bool connectToLc();
    void startTcpReceiver();
    void send(const std::vector<char>& packet);
    void recvLoop();
};
