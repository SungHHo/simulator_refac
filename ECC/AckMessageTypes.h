#pragma once
#include "MessageCommon.h"

#pragma pack(push, 1)

// [0x02] ���̴� ����
struct RadarModeChangeAck {
    CommandType commandType = CommandType::RADAR_MODE_CHANGE;
    unsigned int radar_id;
    uint8_t radar_mode;
    uint8_t response;           // 0=Fail, 1=OK
    char message[256];
};

// [0x03] �߻�� ��� ���� ����
struct LSModeChangeAck {
    CommandType commandType = CommandType::LS_MODE_CHANGE;
    unsigned int ls_id;
    uint8_t ls_mode;
    uint8_t response;
    char message[256];
};

// [0x04] ����ź �߻� ����
struct MissileLaunchAck {
    CommandType commandType = CommandType::MISSILE_LAUNCH;
    unsigned int ls_id;
    uint8_t response;
    char message[256];
};
// [0x05] �߻�� �̵� ����
struct LSMoveAck {
	CommandType commandType = CommandType::LS_MOVE;
	unsigned int ls_id;
	Pos2D new_position;  // 16 byte (x + y)
	uint8_t response;
	char message[256];
};
#pragma pack(pop)
