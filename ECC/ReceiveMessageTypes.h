#pragma once

#include "MessageCommon.h"
#include "RadarStatus.h"
#include "LSStatus.h"
#include "LCStatus.h"
#include "MissileStatus.h"
#include "TargetStatus.h"
#include <vector>
#pragma pack(push, 1)

// ���� ���� ��� (�������� �޽�����)
struct StatusHeader {
    CommandType commandType = CommandType::STATUS_REQUEST;
    uint8_t num_radar;
    uint8_t num_lc;
    uint8_t num_ls;
    uint8_t num_target;
    uint8_t num_missile;
};

#pragma pack(pop)

// ���� ���� �� ����ü�� �ؼ��� ���
struct ParsedStatusResponse {
    std::vector<RadarStatus> radarList;
    std::vector<LCStatus> lcList;
    std::vector<LSStatus> lsList;
    std::vector<TargetStatus> targetList;
    std::vector<MissileStatus> missileList;
};
