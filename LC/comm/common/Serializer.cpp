#include "Serializer.h"
#include <cstring>

#include <iostream>

namespace Common {


//0x01 상태 전송 -> 0x51로 보냄
std::vector<uint8_t> Serializer::serializeStatusResponse(const SystemStatus& status) {
    std::vector<uint8_t> buf;
    buf.push_back(static_cast<uint8_t>(CommandType::STATUS_RESPONSE_LC_TO_ECC)); // 0x51
    buf.push_back(1);  // radar
    buf.push_back(1);  // lc
    buf.push_back(1);  // ls
    buf.push_back(static_cast<uint8_t>(status.targets.size()));
    buf.push_back(static_cast<uint8_t>(status.missiles.size()));

    // MFR
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.mfr.mfrId),
            reinterpret_cast<const uint8_t*>(&status.mfr.mfrId) + 4);
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.mfr.position.x),
            reinterpret_cast<const uint8_t*>(&status.mfr.position.x) + 8);
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.mfr.position.y),
            reinterpret_cast<const uint8_t*>(&status.mfr.position.y) + 8);
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.mfr.height),   
            reinterpret_cast<const uint8_t*>(&status.mfr.height) + 8);
    buf.push_back(static_cast<uint8_t>(status.mfr.mode));
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.mfr.degree),
            reinterpret_cast<const uint8_t*>(&status.mfr.degree) + 8);

        // LS
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.ls.launchSystemId),
                reinterpret_cast<const uint8_t*>(&status.ls.launchSystemId) + 4);

        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.ls.position.x),
                reinterpret_cast<const uint8_t*>(&status.ls.position.x) + 8);

        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.ls.position.y),
                reinterpret_cast<const uint8_t*>(&status.ls.position.y) + 8);

        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.ls.height),  // ✅ height 추가
                reinterpret_cast<const uint8_t*>(&status.ls.height) + 8);

        buf.push_back(static_cast<uint8_t>(status.ls.mode));

        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.ls.launchAngle),
                reinterpret_cast<const uint8_t*>(&status.ls.launchAngle) + 8);

    // LC
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.lc.LCId),
                reinterpret_cast<const uint8_t*>(&status.lc.LCId) + 4);

        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.lc.position.x),
                reinterpret_cast<const uint8_t*>(&status.lc.position.x) + 8);

        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&status.lc.position.y),
                reinterpret_cast<const uint8_t*>(&status.lc.position.y) + 8);
    // std::cout << "[SERIALIZER] missile count: " << status.missiles.size() << "\n";
    // std::cout << "[SERIALIZER] expected total missile bytes: " << status.missiles.size() * 57 << "\n";
    // std::cout << "[SERIALIZER] buffer size before missile: " << buf.size() << "\n";

long long dummyHeight = 15;
buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&dummyHeight),
           reinterpret_cast<const uint8_t*>(&dummyHeight) + sizeof(long long));

    // Missile
    for (const auto& m : status.missiles) {
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&m.id), reinterpret_cast<const uint8_t*>(&m.id) + 4);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&m.posX), reinterpret_cast<const uint8_t*>(&m.posX) + 8);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&m.posY), reinterpret_cast<const uint8_t*>(&m.posY) + 8);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&m.altitude), reinterpret_cast<const uint8_t*>(&m.altitude) + 8);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&m.speed), reinterpret_cast<const uint8_t*>(&m.speed) + 4);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&m.angle), reinterpret_cast<const uint8_t*>(&m.angle) + 8);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&m.detectTime), reinterpret_cast<const uint8_t*>(&m.detectTime) + 8);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&m.interceptTime), reinterpret_cast<const uint8_t*>(&m.interceptTime) + 8);
        buf.push_back(static_cast<uint8_t>(m.hit));
        // std::cout << "[DEBUG] missile ID " << m.id << " serialized, buf size: " << buf.size() << "\n";

    }

    // Target
    for (const auto& t : status.targets) {
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&t.id),         reinterpret_cast<const uint8_t*>(&t.id) + 4);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&t.posX),       reinterpret_cast<const uint8_t*>(&t.posX) + 8);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&t.posY),       reinterpret_cast<const uint8_t*>(&t.posY) + 8);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&t.altitude),   reinterpret_cast<const uint8_t*>(&t.altitude) + 8);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&t.speed),      reinterpret_cast<const uint8_t*>(&t.speed) + 4);
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&t.angle1),     reinterpret_cast<const uint8_t*>(&t.angle1) + 8);  // angle1
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&t.angle2),     reinterpret_cast<const uint8_t*>(&t.angle2) + 8);  // angle2
        buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&t.detectTime), reinterpret_cast<const uint8_t*>(&t.detectTime) + 8);
        buf.push_back(t.priority);
        buf.push_back(static_cast<uint8_t>(t.hit));
    }
    return buf;
}

std::vector<uint8_t> Serializer::serializeLauncherResponse(unsigned int lsId, uint8_t lsMode, bool ok, const std::string& msg) {
    std::vector<uint8_t> buf(260, 0x00);
    buf[0] = 0x03;
    buf[1] = lsId;
    buf[2] = lsMode;
    buf[3] = static_cast<uint8_t>(ok);
    std::memcpy(&buf[4], msg.c_str(), std::min((size_t)256, msg.size()));
    return buf;
}

std::vector<uint8_t> Serializer::serializeLaunchCommand(const LaunchCommand& cmd) {
    std::vector<uint8_t> buf;
    buf.push_back(static_cast<uint8_t>(CommandType::LAUNCH_COMMAND_LC_TO_LS)); // 명령 타입 1바이트

    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&cmd.launcherId),
               reinterpret_cast<const uint8_t*>(&cmd.launcherId) + sizeof(unsigned int));

    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&cmd.launchAngleXY),
               reinterpret_cast<const uint8_t*>(&cmd.launchAngleXY) + sizeof(double));

    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&cmd.launchAngleXZ),
               reinterpret_cast<const uint8_t*>(&cmd.launchAngleXZ) + sizeof(double));

    return buf;
}

std::vector<uint8_t> Serializer::serializeConsoleResponse(uint8_t consoleId, uint8_t mode, bool ok, const std::string& msg) {
    std::vector<uint8_t> buf(260, 0x00);
    buf[0] = 0x05;
    buf[1] = consoleId;
    buf[2] = mode;
    buf[3] = static_cast<uint8_t>(ok);
    std::memcpy(&buf[4], msg.c_str(), std::min((size_t)256, msg.size()));
    return buf;
}

// std::vector<uint8_t> Serializer::serializeRadarResponse(unsigned radarId, uint8_t radarMode, bool ok, const std::string& msg) {
//     std::vector<uint8_t> buf(260, 0x00);
//     buf[0] = static_cast<uint8_t>(CommandType::RADAR_RESPONSE_LC_TO_ECC);  // 예시: 적절한 enum 값 사용
//     std::memcpy(&buf[1], &radarId, sizeof(radarId));       // 4바이트
//     buf[5] = radarMode;
//     buf[6] = static_cast<uint8_t>(ok);
//     std::memcpy(&buf[7], msg.c_str(), std::min(msg.size(), size_t(253))); // 메시지 최대 253바이트
//     return buf;
// }

std::vector<uint8_t> Serializer::serializeMessage(const CommonMessage& msg) {
    std::vector<uint8_t> result;
    result.push_back(static_cast<uint8_t>(msg.commandType));
    return result;
}

std::vector<uint8_t> Serializer::serializeRadarStatusRequest(unsigned int radarId) {
    std::vector<uint8_t> buffer(8, 0x00);
    buffer[0] = static_cast<uint8_t>(CommandType::STATUS_REQUEST_LC_TO_MFR);
    buffer[1] = radarId;
    return buffer;
}

std::vector<uint8_t> Serializer::serializeRadarModeChange(const RadarModeCommand& cmd) {
    std::vector<uint8_t> buffer;

    uint8_t commandType = static_cast<uint8_t>(CommandType::RADAR_MODE_CHANGE_LC_TO_MFR);
    uint8_t radarMode = cmd.radarMode;
    uint8_t priority_or_not = (cmd.targetId == 0) ? 0x01 : 0x02;

    // 로그 출력
    std::cout << "[Serialize] commandType: " << static_cast<int>(commandType)
              << ", radarId: " << cmd.radarId
              << ", radarMode: " << static_cast<int>(radarMode)
              << ", priority_or_not: " << static_cast<int>(priority_or_not)
              << ", targetId: " << cmd.targetId << "\n";

    // 직렬화 시작
    buffer.push_back(commandType); // [0]

    buffer.insert(buffer.end(),
                  reinterpret_cast<const uint8_t*>(&cmd.radarId),
                  reinterpret_cast<const uint8_t*>(&cmd.radarId) + 4); // [1~4]

    buffer.push_back(radarMode); // [5]
    buffer.push_back(priority_or_not); // [6]

    buffer.insert(buffer.end(),
                  reinterpret_cast<const uint8_t*>(&cmd.targetId),
                  reinterpret_cast<const uint8_t*>(&cmd.targetId) + 4); // [7~10]

    return buffer;
}

std::vector<uint8_t> Serializer::serializeLCPositionResponse(const LCPositionResponse& res) {
    std::vector<uint8_t> buf;
    buf.push_back(static_cast<uint8_t>(CommandType::POSITION_RESPONSE_LC_TO_MFR));
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&res.radarId),
               reinterpret_cast<const uint8_t*>(&res.radarId) + sizeof(unsigned int));
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&res.posX),
               reinterpret_cast<const uint8_t*>(&res.posX) + sizeof(long long));
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&res.posY),
               reinterpret_cast<const uint8_t*>(&res.posY) + sizeof(long long));
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&res.height),
               reinterpret_cast<const uint8_t*>(&res.height) + sizeof(long long));
    return buf;
}


std::vector<uint8_t> Serializer::serializeModeChangeCommand(const LauncherModeCommand& cmd) {
    std::vector<uint8_t> buf;
    buf.push_back(static_cast<uint8_t>(CommandType::MODE_CHANGE_LC_TO_LS)); // 명령 타입 1바이트
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&cmd.launcherId),
                          reinterpret_cast<const uint8_t*>(&cmd.launcherId) + sizeof(unsigned int)); // 4
    buf.push_back(static_cast<uint8_t>(cmd.newMode)); // 1
    return buf;
}

//0x05 -> 0x31
std::vector<uint8_t> Serializer::serializeMoveCommandLS(const MoveCommandLS& cmd) {
    std::vector<uint8_t> buf;
    buf.push_back(static_cast<uint8_t>(CommandType::MOVE_COMMAND_LC_TO_LS));
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&cmd.launcherId), reinterpret_cast<const uint8_t*>(&cmd.launcherId) + 4);
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&cmd.newX), reinterpret_cast<const uint8_t*>(&cmd.newX) + 8);
    buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(&cmd.newY), reinterpret_cast<const uint8_t*>(&cmd.newY) + 8);
    return buf;
}



} // namespace Common
