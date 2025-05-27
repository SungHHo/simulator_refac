// MfrSimCommManager.cpp
#include "MfrSimCommManager.hpp"
#include "PacketProtocol.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <cstring>
#include <iostream>

#define SIM_UDP_PORT 9000
#define BUFFER_SIZE 1024

void MfrSimCommManager::setReceiver(IReceiver* recv) 
{
    receiver = recv;
}

void MfrSimCommManager::recv(const std::vector<char>& packet) 
{
    if (receiver) 
    {
        receiver->recvData(packet);
    }
}

void MfrSimCommManager::startUdpReceiver()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
    {
        perror("UDP 소켓 생성 실패");
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SIM_UDP_PORT);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        perror("UDP bind 실패");
        close(sockfd);
        return;
    }

    std::cout << "[SimComm] UDP 수신 대기 중 (port " << SIM_UDP_PORT << ")..." << std::endl;

    std::thread([=]() {
        char buffer[BUFFER_SIZE];

        while (true) 
        {
            sockaddr_in clientAddr{};
            socklen_t addrLen = sizeof(clientAddr);
            ssize_t len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addrLen);

            if (len > 0) 
            {
                if (len == sizeof(uint8_t) + sizeof(SimData)) 
                {
                    SimData data;
                    std::memcpy(&data, buffer, sizeof(SimData));

                    if (receiver) 
                    {
                        std::vector<char> packet(buffer, buffer + len);
                        receiver->recvData(packet);
                    }
                    else
                    {
                        std::cout << "receiver null" << "\n";
                    }
                }
                else
                {
                    std::cerr << "[경고] 예상하지 못한 패킷 크기 (" << len << " bytes)" << std::endl;
                }
            }
        }

        close(sockfd);
    }).detach();
}
