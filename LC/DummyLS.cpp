#include <iostream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

void sendCommand(int sock, const sockaddr_in& targetAddr, uint8_t cmd) {
    std::vector<uint8_t> buf;

    switch (cmd) {
    case 0x01: {  // STATUS_UPDATE
        buf.resize(34);
        buf[0] = 0x41;
        uint32_t lsId = 1;
        double x = 123.456, y = 789.012, angle = 45.0;
        int speed = 30;
        uint8_t mode = 1;

        std::memcpy(&buf[1], &lsId, 4);
        std::memcpy(&buf[5], &x, 8);
        std::memcpy(&buf[13], &y, 8);
        std::memcpy(&buf[21], &speed, 4);
        std::memcpy(&buf[25], &angle, 8);
        buf[33] = mode;
        break;
    }
    case 0x03: {  // MODE_CHANGE 응답
        // 예시: 간단한 성공 여부만 보내기
        buf = {0x03, 0x01};  // cmd, successFlag
        break;
    }
    case 0x05: {  // MOVE_COMMAND
        buf.resize(1 + 4 + 8 + 8);
        buf[0] = 0x05;
        uint32_t lsId = 1;
        double x = 100.0, y = 200.0;
        std::memcpy(&buf[1], &lsId, 4);
        std::memcpy(&buf[5], &x, 8);
        std::memcpy(&buf[13], &y, 8);
        break;
    }
    default:
        std::cerr << "지원되지 않는 명령입니다\n";
        return;
    }

    sendto(sock, buf.data(), buf.size(), 0, (const sockaddr*)&targetAddr, sizeof(targetAddr));
    std::cout << "[DummyLS] 명령 0x" << std::hex << (int)cmd << " 전송 완료 (" << std::dec << buf.size() << " bytes)\n";
}

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in targetAddr{};
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_port = htons(7000);  // LC가 바인딩한 포트
    inet_pton(AF_INET, "127.0.0.1", &targetAddr.sin_addr);  // LC가 동작하는 IP

    while (true) {
        std::cout << "\n전송할 명령 선택: 1=STATUS, 3=MODE_CHANGE 응답, 5=MOVE, 0=종료 >> ";
        int cmd;
        std::cin >> cmd;
        if (cmd == 0) break;

        sendCommand(sock, targetAddr, static_cast<uint8_t>(cmd));
    }

    close(sock);
    return 0;
}
