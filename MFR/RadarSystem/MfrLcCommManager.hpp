#pragma once
#include "IReceiver.hpp"

#include "MfrConfig.h"
#include "ini.h"

#include <vector>

class MfrLcCommManager {
private:
    const unsigned int mfrId = 1;
    int sockfd;
    IReceiver* receiver;
    
    MfrConfig mfrConfig;
    std::string lcIp;
    int lcPort;

public:
    MfrLcCommManager(IReceiver* receiver);
    void send(const std::vector<char>& packet);

private:
    void initMfrLcCommManager();
    bool connectToLc();
    void startTcpReceiver();    
};
