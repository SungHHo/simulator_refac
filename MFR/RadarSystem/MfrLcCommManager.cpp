#include "MfrLcCommManager.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>

#define LC_SERVER_IP "127.0.0.1"
#define LC_SERVER_PORT 7000
#define BUFFER_SIZE 1024

MfrLcCommManager::MfrLcCommManager(IReceiver* receiver)
    : receiver(receiver), sockfd(-1) {}

bool MfrLcCommManager::connectToLc() 
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("소켓 생성 실패");
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(LC_SERVER_PORT);
    inet_pton(AF_INET, LC_SERVER_IP, &serverAddr.sin_addr);

    if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        perror("LC 서버 연결 실패");
        close(sockfd);
        sockfd = -1;
        return false;
    }

    std::cout << "[MfrLcCommManager] LC 서버에 연결 성공" << std::endl;
    return true;
}

void MfrLcCommManager::startTcpReceiver()
{
    if (sockfd < 0)
    {
        std::cerr << "[MfrLcCommManager] 유효하지 않은 소켓" << std::endl;
        return;
    }

    std::thread([=]() {
        char buffer[BUFFER_SIZE];

        std::cout << "[TcpComm] TCP 수신 대기 시작..." << std::endl;

        while (true)
        {
            ssize_t len = read(sockfd, buffer, sizeof(buffer));
            if (len <= 0)
            {
                std::cerr << "[TcpComm] 연결 종료 또는 수신 실패" << std::endl;
                break;
            }

            std::vector<char> packet(buffer, buffer + len);
            std::cout << "[TcpComm] 수신된 패킷: " << len << " bytes" << std::endl;

            if (receiver)
            {
                receiver->recvData(packet);
            }
            else
            {
                std::cerr << "[TcpComm] receiver가 null입니다." << std::endl;
            }
        }

        close(sockfd);
    }).detach();
}


void MfrLcCommManager::send(const std::vector<char>& packet) 
{
    if (sockfd < 0) 
    {
        std::cerr << "[MfrLcCommManager] 소켓이 미 Open" << std::endl;
        return;
    }

    ssize_t sent = write(sockfd, packet.data(), packet.size());
    if (sent != packet.size()) 
    {
        std::cerr << "[MfrLcCommManager] 전송 실패" << std::endl;
    }

    else 
    {
        std::cout << "[MfrLcCommManager] send() 성공, size: " << sent << " bytes" << std::endl;
    }
}

void MfrLcCommManager::recvLoop() 
{
    if (sockfd < 0) return;

    std::thread([this]() {
        char buffer[1024];
        while (true) {
            ssize_t len = read(sockfd, buffer, sizeof(buffer));
            if (len <= 0) break;

            std::vector<char> packet(buffer, buffer + len);
            std::cout << "[MfrLcCommManager] recv() 호출됨, size: " << len << std::endl;

            if (receiver) 
            {
                receiver->recvData(packet);  // 필요 시 ID 처리
            }
        }
    }).detach();
}
