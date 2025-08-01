#include "Packet.h"
#include "ByteBuffer.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    const int port = 9999;               // MFR.ini [LaunchController] Port 와 동일해야 함
    const int backlog = 1;

    // 1) 서버 소켓 생성
    int listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0) {
        perror("socket");
        return 1;
    }

    // 2) SO_REUSEADDR 옵션
    int opt = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 3) bind
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(listenSock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(listenSock);
        return 1;
    }

    // 4) listen
    if (listen(listenSock, backlog) < 0) {
        perror("listen");
        close(listenSock);
        return 1;
    }
    std::cout << "[TestServer] Listening on port " << port << "\n";

    // 5) accept
    int clientSock = accept(listenSock, nullptr, nullptr);
    if (clientSock < 0) {
        perror("accept");
        close(listenSock);
        return 1;
    }
    std::cout << "[TestServer] Client connected\n";

    // 6) Packet 생성 (예시 값)
    Packet pkt;
    pkt.h1 = {1, 2, 1, 0, 1111, 2222};
    pkt.h2 = {1, 2, MSG_TYPE_COMMAND, 0, 1, 1, 1111, 2222};
    pkt.payload2 = {
        'A','B', MODE_MOVE,
        37.0f, 127.0f, 50.0f,
        10.0f, 20.0f, true
    };

    // 7) 직렬화
    ByteBuffer buf;
    pkt.serialize(buf);
    const std::vector<byte> data = buf.data();

    // 8) 10Hz 주기로 데이터 전송
    while (true) {
        ssize_t sent = write(clientSock, data.data(), data.size());
        if (sent < 0) {
            perror("write");
            break;
        }
        std::cout << "[TestServer] Sent " << sent << " bytes\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    close(clientSock);
    close(listenSock);
    return 0;
}
