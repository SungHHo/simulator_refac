#pragma once
#include <cstdint>

#pragma pack(push, 1)

enum class CommandType : uint8_t {
    STATUS_REQUEST = 0x01,
    RADAR_MODE_CHANGE = 0x02,
    LS_MODE_CHANGE = 0x03,
    MISSILE_LAUNCH = 0x04
};

struct Pos2D {
    long long x;
    long long y;
};

struct StatusHeader {
    CommandType commandType = CommandType::STATUS_REQUEST;
    uint8_t num_radar;
    uint8_t num_lc;
    uint8_t num_ls;
    uint8_t num_target;
    uint8_t num_missile;
};

struct RadarStatus {
    uint8_t id;
    Pos2D position;
    uint8_t mode;
    double angle;
};

struct LSStatus {
    uint8_t id;
    Pos2D position;
    uint8_t mode;
    double angle;
};

struct LCStatus {
    uint8_t id;
    Pos2D position;
};

struct MissileStatus {
    uint8_t id;
    Pos2D position;
    int64_t height;
    int64_t speed;
    double angle;
    int64_t predicted_time;
    uint8_t hit;
};

struct TargetStatus {
    uint8_t id;
    Pos2D position;
    int64_t height;
    int64_t speed;
    double angle;
    int64_t first_detect_time;
    int32_t priority;
    uint8_t hit;
};

#pragma pack(pop)
