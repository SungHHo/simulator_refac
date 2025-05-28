#include <iostream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

std::vector<uint8_t> makeStatusResponse() {
    std::vector<uint8_t> buf;
    buf.push_back(0x21);  // CommandType::STATUS_RESPONSE_MFR_TO_LC

    uint32_t radarId = 1;
    long long posX = 1000;
    long long posY = 2000;
    uint8_t mode = 1;
    double angle = 45.0;
    uint32_t mockTargetId = 11;
    uint32_t mockMissileId = 22;

    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&radarId), reinterpret_cast<uint8_t*>(&radarId) + 4);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&posX), reinterpret_cast<uint8_t*>(&posX) + 8);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&posY), reinterpret_cast<uint8_t*>(&posY) + 8);
    buf.push_back(mode);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&angle), reinterpret_cast<uint8_t*>(&angle) + 8);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&mockTargetId), reinterpret_cast<uint8_t*>(&mockTargetId) + 4);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&mockMissileId), reinterpret_cast<uint8_t*>(&mockMissileId) + 4);

    return buf;
}

std::vector<uint8_t> makeDetection() {
    std::vector<uint8_t> buf;
    buf.push_back(0x22);  // CommandType::DETECTION_MFR_TO_LC

    uint32_t radarId = 1;
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&radarId), reinterpret_cast<uint8_t*>(&radarId) + 4);

    // 간단히 타겟 1기
    uint32_t targetId = 1;
    long long posX = 1000, posY = 2000, altitude = 500;
    int speed = 300;
    double angle = 60.0;
    long long detectTime = 123456789;
    uint8_t priority = 1;
    uint8_t hit = 0;

    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&targetId), reinterpret_cast<uint8_t*>(&targetId) + 4);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&posX), reinterpret_cast<uint8_t*>(&posX) + 8);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&posY), reinterpret_cast<uint8_t*>(&posY) + 8);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&altitude), reinterpret_cast<uint8_t*>(&altitude) + 8);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&speed), reinterpret_cast<uint8_t*>(&speed) + 4);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&angle), reinterpret_cast<uint8_t*>(&angle) + 8);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&detectTime), reinterpret_cast<uint8_t*>(&detectTime) + 8);
    buf.push_back(priority);
    buf.push_back(hit);

    // 미사일 1기
    uint32_t missileId = 5;
    posX = 1500; posY = 1800; altitude = 300;
    speed = 400;
    angle = 30.0;
    detectTime = 987654321;
    hit = 0;

    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&missileId), reinterpret_cast<uint8_t*>(&missileId) + 4);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&posX), reinterpret_cast<uint8_t*>(&posX) + 8);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&posY), reinterpret_cast<uint8_t*>(&posY) + 8);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&altitude), reinterpret_cast<uint8_t*>(&altitude) + 8);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&angle), reinterpret_cast<uint8_t*>(&angle) + 8);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&speed), reinterpret_cast<uint8_t*>(&speed) + 4);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&detectTime), reinterpret_cast<uint8_t*>(&detectTime) + 8);
    buf.push_back(hit);

    return buf;
}

std::vector<uint8_t> makePositionRequest() {
    return { 0x23 };  // CommandType::POSITION_REQUEST_MFR_TO_LC
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "연결 실패\n";
        return 1;
    }

    while (true) {
        int cmd;
        std::cout << "MFR 명령 입력 (21=상태, 22=탐지, 23=위치요청, 0=종료): ";
        std::cin >> cmd;

        std::vector<uint8_t> packet;
        if (cmd == 0) break;
        else if (cmd == 21) packet = makeStatusResponse();
        else if (cmd == 22) packet = makeDetection();
        else if (cmd == 23) packet = makePositionRequest();
        else continue;

        send(sock, packet.data(), packet.size(), 0);
        std::cout << "[MFR Dummy] 전송 완료 (" << packet.size() << " bytes)\n";
    }

    close(sock);
    return 0;
}
