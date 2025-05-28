#pragma once

#include <vector>
#include <cstdint>
#include <cstring>

// 1바이트 정렬 시작
#pragma pack(push, 1)

enum class OperationMode : uint8_t {
    WAR_MODE = 0,
    MOVE_MODE = 1,
    STOP_MODE = 2
};

enum class CommandType : uint8_t {
    LAUNCH         = 0x31, // 발사명령
    MOVE           = 0x32, // 이동명령
    MODE_CHANGE    = 0x33, // 모드변경
    STATUS_REQUEST = 0x34  // 상태요청
};

struct MissileInfo {
    int64_t LS_pos_x;
    int64_t LS_pos_y;
    int64_t LS_pos_z;
    int32_t speed;
    double degree;

    std::vector<uint8_t> toBytes() const {
        std::vector<uint8_t> buffer;
        buffer.reserve(sizeof(*this));

        auto append = [&buffer](const void* data, size_t size) {
            const uint8_t* bytes = static_cast<const uint8_t*>(data);
            buffer.insert(buffer.end(), bytes, bytes + size);
        };

        append(&LS_pos_x, sizeof(LS_pos_x));
        append(&LS_pos_y, sizeof(LS_pos_y));
        append(&LS_pos_z, sizeof(LS_pos_z));
        append(&speed, sizeof(speed));
        append(&degree, sizeof(degree));

        return buffer;
    }
};

struct LaunchCommand {
    double launch_angle;
    int32_t speed;    
    int64_t altitude;
};

struct MoveCommand {
    int64_t new_x;
    int64_t new_y;
};

struct ModeChangeCommand {
    OperationMode new_mode;
};

struct LauncherMessage {
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
