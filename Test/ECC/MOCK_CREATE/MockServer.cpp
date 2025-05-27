#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "SendMessageTypes.h"
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")  // Winsock 라이브러리 링크

std::string ReadIni(const std::string& section, const std::string& key, const std::string& path) {
    char buffer[256] = {};
    GetPrivateProfileStringA(section.c_str(), key.c_str(), "", buffer, sizeof(buffer), path.c_str());
    return std::string(buffer);
}

int ReadInt(const std::string& section, const std::string& key, const std::string& path) {
    return GetPrivateProfileIntA(section.c_str(), key.c_str(), 0, path.c_str());
}

long long ReadLongLong(const std::string& section, const std::string& key,
                       const std::string& path) {
  std::string str = ReadIni(section, key, path);
  return str.empty() ? 0LL : std::stoll(str);
}

double ReadDouble(const std::string& section, const std::string& key, const std::string& path) {
    std::string str = ReadIni(section, key, path);
    return str.empty() ? 0.0 : atof(str.c_str());
}

void SendSystemStatus(SOCKET client, const std::string& iniPath) {
    // 1. INI 읽기
    RadarStatus radar;
    radar.id = (uint8_t)ReadInt("MFR", "mfrId", iniPath);
    radar.position = { ReadInt("MFR", "posX", iniPath), ReadInt("MFR", "posY", iniPath) };
    radar.mode = (uint8_t)ReadInt("MFR", "mode", iniPath);
    radar.angle = ReadDouble("MFR", "degree", iniPath);
    

    LSStatus ls;
    ls.id = (uint8_t)ReadInt("LS", "launchSystemId", iniPath);
    ls.position = { ReadInt("LS", "posX", iniPath), ReadInt("LS", "posY", iniPath) };
    ls.mode = (uint8_t)ReadInt("LS", "mode", iniPath);
    ls.angle = ReadDouble("LS", "launchAngle", iniPath);

    LCStatus lc;
    lc.id = (uint8_t)ReadInt("LC", "commandReady", iniPath);
    lc.position = { ReadInt("LC", "posX", iniPath), ReadInt("LC", "posY", iniPath) };

    MissileStatus missile;
    missile.id = (uint8_t)ReadInt("Missile", "missileId", iniPath);
    missile.position = {ReadLongLong("Missile", "posX", iniPath),
                        ReadLongLong("Missile", "posY", iniPath)};
    missile.height = ReadInt("Missile", "height", iniPath);
    missile.speed = ReadInt("Missile", "speed", iniPath);
    missile.angle = ReadDouble("Missile", "degree", iniPath);
    missile.predicted_time = ReadInt("Missile", "predicted_time", iniPath);
    missile.hit = (uint8_t)ReadInt("Missile", "hit", iniPath);

    std::vector<TargetStatus> targets;
    for (int i = 0; i < 100; ++i) {
        std::string section = "Target_" + std::to_string(i);
        int rawId = ReadInt(section, "targetId", iniPath);
        if (rawId == 0) break;

        if (rawId < 0 || rawId > 255) {
            std::cerr << "[경고] Target_" << i << ": ID(" << rawId << ")는 uint8_t 범위를 벗어났습니다. 건너뜁니다.\n";
            continue;
        }

        TargetStatus t{};
        t.id = static_cast<uint8_t>(rawId);
        t.position = {
            ReadInt(section, "posX", iniPath),
            ReadInt(section, "posY", iniPath)
        };
        t.height = ReadInt(section, "height", iniPath);
        t.speed = ReadInt(section, "speed", iniPath);
        t.angle = ReadDouble(section, "degree", iniPath);
        t.first_detect_time = ReadInt(section, "first_detect_time", iniPath);
        t.priority = ReadInt(section, "priority", iniPath);
        t.hit = static_cast<uint8_t>(ReadInt(section, "hit", iniPath));

        targets.push_back(t);
    }


    // 2. 헤더 구성
    StatusHeader header;
    header.commandType = CommandType::STATUS_REQUEST;
    header.num_radar = 1;
    header.num_lc = 1;
    header.num_ls = 1;
    header.num_target = (uint8_t)targets.size();
    header.num_missile = 1;

    // 3. 버퍼 구성
    std::vector<uint8_t> buffer;
    auto append = [&](const void* data, size_t size) {
        buffer.insert(buffer.end(), (const uint8_t*)data, (const uint8_t*)data + size);
    };

    append(&header, sizeof(header));
    append(&radar, sizeof(radar));
    append(&lc, sizeof(lc));
    append(&ls, sizeof(ls));
    for (const auto& t : targets) {
        append(&t, sizeof(t));
    }
    append(&missile, sizeof(missile));

    // 4. 고정 크기 3072 바이트 패딩
    //if (buffer.size() < 3072) {
    //    buffer.resize(3072, 0);  // 0으로 채움
    //}

    // 5. 전송
    send(client, reinterpret_cast<const char*>(buffer.data()), buffer.size(), 0);
    std::cout << "[DEBUG] sizeof(RadarStatus) = " << sizeof(RadarStatus) << "\n";
    std::cout << "[DEBUG] sizeof(LCStatus) = " << sizeof(LCStatus) << "\n";
    std::cout << "[DEBUG] sizeof(LSStatus) = " << sizeof(LSStatus) << "\n";
    std::cout << "[DEBUG] sizeof(MissileStatus) = " << sizeof(MissileStatus) << "\n";
    std::cout << "[DEBUG] sizeof(TargetStatus) = " << sizeof(TargetStatus) << "\n";
    std::cout << "[MockServer] Send Complete (" << buffer.size() << " bytes, " << targets.size() << " targets)\n";
}




int main() {
    std::string iniPath = "C:\\Users\\user\\Desktop\\Surface-to-air-missiles\\Test\\ECC\\MOCK_CREATE\\config.ini";

    WSADATA wsaData;
    SOCKET sock;
    sockaddr_in serverAddr;
    const char* serverIP = "127.0.0.1";  // 서버 IP (로컬호스트 예제)
    int serverPort = 9001;               // 서버 포트

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      std::cerr << "WSAStartup failed!" << std::endl;
      return 1;
    }

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
      std::cerr << "Socket creation failed!" << std::endl;
      WSACleanup();
      return 1;
    }

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    InetPtonA(AF_INET, serverIP, &serverAddr.sin_addr);

    // 서버에 연결
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) ==
        SOCKET_ERROR) {
      std::cerr << "Connection failed!" << std::endl;
      closesocket(sock);
      WSACleanup();
      return 1;
    }

    while (true) {
        char buffer[1];
        SendSystemStatus(sock, iniPath);
        Sleep(1000);

      //int len = recv(sock, buffer, sizeof(buffer), 0);
      //  if (len <= 0) break;
      //  if ((uint8_t)buffer[0] == (uint8_t)CommandType::STATUS_REQUEST) {
      //  }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
