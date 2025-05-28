#include <iostream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

// 구조체 정의 생략 – 필요한 필드만 직렬화
std::vector<uint8_t> makeStatusRequest() {
    return { 0x01 };  // CommandType::STATUS_REQUEST_ECC_TO_LC
}

std::vector<uint8_t> makeSetRadarMode() {
    std::vector<uint8_t> buf;
    buf.push_back(0x02);  // SET_RADAR_MODE_ECC_TO_LC
    uint32_t radarId = 1;
    uint8_t mode = 0;     // STOP
    uint8_t flag = 0x02;
    uint32_t targetId = 1234;

    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&radarId), reinterpret_cast<uint8_t*>(&radarId) + 4);
    buf.push_back(mode);
    buf.push_back(flag);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&targetId), reinterpret_cast<uint8_t*>(&targetId) + 4);
    return buf;
}

std::vector<uint8_t> makeSetLauncherMode() {
    std::vector<uint8_t> buf;
    buf.push_back(0x03);  // SET_LAUNCHER_MODE_ECC_TO_LC
    uint32_t lsId = 2;
    uint8_t mode = 1; // 예시로 ACTIVE

    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&lsId), reinterpret_cast<uint8_t*>(&lsId) + 4);
    buf.push_back(mode);
    return buf;
}

std::vector<uint8_t> makeFireCommand() {
    std::vector<uint8_t> buf;
    buf.push_back(0x04);  // FIRE_COMMAND_ECC_TO_LC
    uint8_t lsId = 2;
    uint32_t targetId = 1234;

    buf.push_back(lsId);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&targetId), reinterpret_cast<uint8_t*>(&targetId) + 4);
    return buf;
}

std::vector<uint8_t> makeMoveCommand() {
    std::vector<uint8_t> buf;
    buf.push_back(0x05);  // MOVE_COMMAND_ECC_TO_LC
    uint32_t lsId = 2;
    double posX = 100.0;
    double posY = 200.0;

    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&lsId), reinterpret_cast<uint8_t*>(&lsId) + 4);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&posX), reinterpret_cast<uint8_t*>(&posX) + 8);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&posY), reinterpret_cast<uint8_t*>(&posY) + 8);
    return buf;
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888); // LC에서 ECC listen 중인 포트
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr); // 로컬에서 테스트

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "연결 실패\n";
        return 1;
    }

    while (true) {
        int cmd;
        std::cout << "명령 입력 (1~5, 0=종료): ";
        std::cin >> cmd;

        std::vector<uint8_t> packet;
        if (cmd == 0) break;
        else if (cmd == 1) packet = makeStatusRequest();
        else if (cmd == 2) packet = makeSetRadarMode();
        else if (cmd == 3) packet = makeSetLauncherMode();
        else if (cmd == 4) packet = makeFireCommand();
        else if (cmd == 5) packet = makeMoveCommand();
        else continue;

        send(sock, packet.data(), packet.size(), 0);
        std::cout << "[ECC Dummy] 전송 완료 (" << packet.size() << " bytes)\n";
    }

    close(sock);
    return 0;
}
