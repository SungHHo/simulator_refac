#pragma once
#include <cstdint>

// 1. Command Type ���� (�ۼ��� ���� ���� 0x01~0x05 ����)
enum class CommandType : uint8_t {
    STATUS_REQUEST = 0x01,
    RADAR_MODE_CHANGE = 0x02,
    LS_MODE_CHANGE = 0x03,
    MISSILE_LAUNCH = 0x04,
	LS_MOVE = 0x05
};

struct Pos2D {
    long long x;
    long long y;
};
