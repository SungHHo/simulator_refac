#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <random>
#include "PacketProtocol.hpp"

#define SERVER_IP   "127.0.0.1"  // MFR 수신 IP
#define SERVER_PORT 9000         // MFR 수신 포트

int main() 
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("소켓 생성 실패");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // 랜덤 엔진 설정
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> typeDist(0, 1);  // 0: 표적, 1: 미사일
    std::uniform_int_distribution<int> targetDist(101001, 101999);
    std::uniform_int_distribution<int> missileDist(102001, 102999);

    uint32_t count = 1;

    while (true) 
    {
        SimData data{};

        int type = typeDist(gen);
        //std::cout << type << "\n";
        data.cmd = type;
        data.id = (type == 0) ? targetDist(gen) : missileDist(gen);
        data.x = 1000.0f + count * 10;
        data.y = 2000.0f + count * 10;
        data.z = 300.0f;
        data.angle = 45.0f + count;
        data.speed = 250 + (count % 50);

        ssize_t sent = sendto(sock, &data, sizeof(SimData), 0,
                              reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

        if (sent == sizeof(SimData)) 
        {
            std::cout << "[Sim] CMD TYPE: " << (int)data.cmd << " / ID: " << data.id << " / 데이터 전송 완료 (" << sent << " bytes)" << std::endl;
        }
        else 
        {
            std::cerr << "[Sim] 전송 실패" << std::endl;
        }

        ++count;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(sock);
    return 0;
}
