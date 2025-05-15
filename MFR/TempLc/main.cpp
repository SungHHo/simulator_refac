#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 7000
#define REQUEST_CODE 0xA1
#define BUFFER_SIZE 1024

int main()
{
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) 
    {
        perror("소켓 생성 실패");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        perror("bind 실패");
        return 1;
    }

    if (listen(serverSock, 1) < 0) 
    {
        perror("listen 실패");
        return 1;
    }

    std::cout << "[TempLC] TCP 서버 시작, 연결 대기 중..." << std::endl;

    sockaddr_in clientAddr{};
    socklen_t addrLen = sizeof(clientAddr);
    int clientSock = accept(serverSock, (sockaddr*)&clientAddr, &addrLen);
    if (clientSock < 0) 
    {
        perror("accept 실패");
        return 1;
    }

    std::cout << "[LC] MFR와 연결됨" << std::endl;

    while (true) 
    {
        std::vector<char> requestPacket(1);
        requestPacket[0] = REQUEST_CODE;
     
        send(clientSock, requestPacket.data(), requestPacket.size(), 0);
        std::cout << "[LC] 레이더 상태 요청 전송" << std::endl;

        

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    close(clientSock);
    close(serverSock);
    return 0;
}
