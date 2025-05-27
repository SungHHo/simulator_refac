// Deserializer.cpp
#include "pch.h"
#include "Deserializer.h"
#include <cstring>
#include <iostream>
bool DeserializeRadarModeAck(const uint8_t* data, size_t len, RadarModeChangeAck& out)
{
    if (len < sizeof(RadarModeChangeAck)) return false;
    std::memcpy(&out, data, sizeof(RadarModeChangeAck));
    return true;
}

bool DeserializeLSModeAck(const uint8_t* data, size_t len, LSModeChangeAck& out)
{
    if (len < sizeof(LSModeChangeAck)) return false;
    std::memcpy(&out, data, sizeof(LSModeChangeAck));
    return true;
}

bool DeserializeMissileAck(const uint8_t* data, size_t len, MissileLaunchAck& out)
{
    if (len < sizeof(MissileLaunchAck)) return false;
    std::memcpy(&out, data, sizeof(MissileLaunchAck));
    return true;
}
bool DeserializeLSMoveAck(const uint8_t* data, size_t len, LSMoveAck& out)
{
	if (len < sizeof(LSMoveAck)) return false;
	std::memcpy(&out, data, sizeof(LSMoveAck));
	return true;
}
bool DeserializeStatusResponse(
    const uint8_t* data, size_t len,
    std::vector<RadarStatus>& radars,
    std::vector<LCStatus>& lcs,
    std::vector<LSStatus>& lss,
    std::vector<TargetStatus>& targets,
    std::vector<MissileStatus>& missiles
)
{
    size_t offset = 0;

    if (len < sizeof(StatusHeader)) return false;

    const StatusHeader* header = reinterpret_cast<const StatusHeader*>(data);
    offset += sizeof(StatusHeader);

    // 총 예상 바이트 수
    size_t expectedSize =
        sizeof(StatusHeader)
        + header->num_radar * sizeof(RadarStatus)
        + header->num_lc * sizeof(LCStatus)
        + header->num_ls * sizeof(LSStatus)
        + header->num_target * sizeof(TargetStatus)
        + header->num_missile * sizeof(MissileStatus);
	std::cout << "[디버그] 예상 크기: " << expectedSize << ", 실제 크기: " << len << "\n";

    if (len < expectedSize) return false;

    // 안전한 배열 복사 람다
    auto read_array = [&](auto& vec, size_t count, size_t itemSize) -> bool {
        if (offset + count * itemSize > len) return false;

        for (size_t i = 0; i < count; ++i) {
            typename std::remove_reference_t<decltype(vec)>::value_type item{};
            std::memcpy(&item, data + offset, itemSize);
            vec.push_back(item);
            offset += itemSize;
        }
        return true;
        };

    if (!read_array(radars, header->num_radar, sizeof(RadarStatus))) return false;
    if (!read_array(lcs, header->num_lc, sizeof(LCStatus))) return false;
    if (!read_array(lss, header->num_ls, sizeof(LSStatus))) return false;
    if (!read_array(targets, header->num_target, sizeof(TargetStatus))) return false;
    if (!read_array(missiles, header->num_missile, sizeof(MissileStatus))) return false;

    return true;
}

