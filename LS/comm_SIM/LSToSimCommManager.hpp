#pragma once
#include <string>
#include <netinet/in.h>
#include <vector>
#include "LSStatusManager.hpp"
#include "info.hpp"

class LSToSimCommManager
{
private:
    int fd;
    std::string IP;
    int port;
    sockaddr_in address;
    int init(const std::string& configPath);
    
public:
    explicit LSToSimCommManager(const std::string& configPath);
    ~LSToSimCommManager();

    void sendMissileInfo(const LaunchCommand& command, const LSStatusManager& statManager);
    void stop();
};
