#pragma once

#include "MessageCommon.h"  // Pos2D

#pragma pack(push, 1)

struct TargetStatus {
    uint32_t id;
    Pos2D position;
    int64_t height;
    int64_t speed;
    double angle;
    int64_t first_detect_time;
    int32_t priority;
    uint8_t hit;
};

#pragma pack(pop)
