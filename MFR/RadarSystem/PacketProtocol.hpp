#pragma once
#include <cstdint>
#pragma pack(push, 1)

struct PacketHeader 
{
    uint8_t cmdType;
};

struct SimData 
{
    uint32_t mockId;
    float x;
    float y;
    float z;
    float angle;
    int speed;
};

enum PacketType : uint8_t 
{
    SIM_TARGET_DATA = 0,
    SIM_MISSILE_DATA = 1,
    STATUS_REQ = 0xA1,
    STATUS_RESP = 0xA2,
};