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
    unsigned int radar_id;
    uint8_t radar_mode;
	uint8_t priority_select; // 1= �ֿ켱 �켱����, 2= ����� ���� �켱����
    unsigned int target_id;
};

// [0x03] �߻�� ��� ���� ? 19 byte
struct LSModeChange {
    CommandType commandType = CommandType::LS_MODE_CHANGE;
    unsigned int ls_id;
    uint8_t ls_mode;
};

// [0x04] ����ź �߻� ? 3 byte
struct MissileLaunch {
    CommandType commandType = CommandType::MISSILE_LAUNCH;
    unsigned int ls_id;
    unsigned int target_id;
};
// [0x05] �߻�� �̵� ? 18 byte
struct LSMove {
	CommandType commandType = CommandType::LS_MOVE;
	unsigned int ls_id;
	Pos2D position;  // 16 byte (x + y)
};

#pragma pack(pop)
