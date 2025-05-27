#pragma once
#include "MessageCommon.h"

#pragma pack(push, 1)

// [0x01] ���� ��û ? 1 byte
struct StatusRequest {
    CommandType commandType = CommandType::STATUS_REQUEST;
};

// [0x02] ���̴� ��� ���� ? 4 byte
struct RadarModeChange {
    CommandType commandType = CommandType::RADAR_MODE_CHANGE;
    uint8_t radar_id;
    uint8_t radar_mode;
    uint8_t target_id;
};

// [0x03] �߻�� ��� ���� ? 19 byte
struct LSModeChange {
    CommandType commandType = CommandType::LS_MODE_CHANGE;
    uint8_t ls_id;
    uint8_t ls_mode;
};

// [0x04] ����ź �߻� ? 3 byte
struct MissileLaunch {
    CommandType commandType = CommandType::MISSILE_LAUNCH;
    uint8_t ls_id;
    uint8_t target_id;
};
// [0x05] �߻�� �̵� ? 18 byte
struct LSMove {
	CommandType commandType = CommandType::LS_MOVE;
	uint8_t ls_id;
	Pos2D position;  // 16 byte (x + y)
};

#pragma pack(pop)
