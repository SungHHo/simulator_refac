#pragma once

#include "MessageCommon.h"  // Pos2D

#pragma pack(push, 1)

struct LCStatus {
    uint8_t id;
    Pos2D position;
};

#pragma pack(pop)
