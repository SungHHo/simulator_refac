#pragma once

#include <cstdint>

#pragma pack(push, 1)
struct SimData 
{
    uint8_t cmd;
    uint32_t id;
    float x;
    float y;
    float z;
    float angle;
    int speed;
};
#pragma pack(pop)