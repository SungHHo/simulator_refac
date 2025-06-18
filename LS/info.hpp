#pragma once

#include <vector>
#include <cstdint>
#include <cstring>

// 패딩 제거
#pragma pack(push, 1)

enum class OperationMode : uint8_t
{
    WAR_MODE = 0,
    MOVE_MODE = 1,
    STOP_MODE = 2
};

enum class CommandType : uint8_t
{
    LAUNCH         = 0x31, // 발사명령
    MOVE           = 0x32, // 이동명령
    MODE_CHANGE    = 0x33, // 모드변경
    STATUS_REQUEST = 0x34  // 상태요청
};

struct MissileInfo
{
    int64_t LS_pos_x;
    int64_t LS_pos_y;
    int64_t LS_pos_z;
    int speed;
    double degree_xy;
    double degree_xz;

    // int64_t를 big-endian 바이트로 변환
    static void appendInt64(std::vector<uint8_t>& buffer, int64_t value)
    {
        for (int i = 0; i < 8; ++i) {
            buffer.push_back((value >> (56 - i * 8)) & 0xFF);
        }
    }

    // int를 big-endian 바이트로 변환
    static void appendInt(std::vector<uint8_t>& buffer, int value)
    {
        for (int i = 0; i < 4; ++i) {
            buffer.push_back((value >> (24 - i * 8)) & 0xFF);
        }
    }

    // double을 8바이트로 변환
    static void appendDouble(std::vector<uint8_t>& buffer, double value)
    {
        uint64_t bits;
        std::memcpy(&bits, &value, sizeof(bits));
        appendInt64(buffer, static_cast<int64_t>(bits));
    }

    // 직렬화 함수
    std::vector<uint8_t> toBytes() const 
    {
        std::vector<uint8_t> buffer;
        buffer.reserve(sizeof(MissileInfo));

        appendInt64(buffer, LS_pos_x);
        appendInt64(buffer, LS_pos_y);
        appendInt64(buffer, LS_pos_z);
        appendInt(buffer, speed);
        appendDouble(buffer, degree_xy);
        appendDouble(buffer, degree_xz);

        return buffer;
    }
};

struct LaunchCommand
{
    double launch_angle_xy;
    double launch_angle_xz;
};

struct MoveCommand
{
    int64_t new_x;
    int64_t new_y;
};

struct ModeChangeCommand
{
    OperationMode new_mode;
};

struct LauncherMessage
{
    CommandType type;
    uint32_t launcher_id;
    union {
        LaunchCommand launch;
        MoveCommand move;
        ModeChangeCommand mode_change;
    };
};

// 정렬 복원
#pragma pack(pop)