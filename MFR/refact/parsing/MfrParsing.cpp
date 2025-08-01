#include "MfrParsing.h"
#include <iostream>

bool Mfr::parseIncomingData(const std::vector<byte>& rawData)
{
    ByteBuffer buffer;
    buffer.setBuffer(rawData);

    Packet packet;
    packet.deserialize(buffer);

    switch (packet.h2.msgType)
    {
        case MSG_TYPE_COMMAND:
            handleCommand(packet);
            break;
        case MSG_TYPE_STATUS:
            handleStatus(packet);
            break;
        default:
            std::cerr << "[MFR] 알 수 없는 메시지 타입: "
                      << static_cast<int>(packet.h2.msgType) << '\n';
            return false;
    }

    return true;
}

void Mfr::handleCommand(const Packet& pkt)
{
    std::cout << "[CMD] ID: " << static_cast<int>(pkt.payload2.id)
              << ", Mode: " << static_cast<int>(pkt.payload2.mode)
              << ", Lat: " << pkt.payload2.lat
              << ", Lon: " << pkt.payload2.lon
              << ", Alt: " << pkt.payload2.alt
              << std::endl;
}

void Mfr::handleStatus(const Packet& pkt)
{
    std::cout << "[STATUS] ID: " << static_cast<int>(pkt.payload2.id)
              << ", Angle1: " << pkt.payload2.angle1
              << ", Angle2: " << pkt.payload2.angle2
              << ", isTrue: " << std::boolalpha << pkt.payload2.isTrue
              << std::endl;
}
