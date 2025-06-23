// Serializer.h
#pragma once
#include <vector>
#include <cstdint>
#include "SendMessageTypes.h"  // �۽� �޽��� ����ü ����

// �� �޽����� ����ȭ �Լ�
std::vector<uint8_t> SerializeRadarModeChange(const RadarModeChange& msg);
std::vector<uint8_t> SerializeLSModeChange(const LSModeChange& msg);
std::vector<uint8_t> SerializeMissileLaunch(const MissileLaunch& msg);
std::vector<uint8_t> SerializeStatusRequest(const StatusRequest& msg);
std::vector<uint8_t> SerializeLSMove(const LSMove& msg);