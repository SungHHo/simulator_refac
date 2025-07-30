#ifndef PAYLOAD2_H
#define PAYLOAD2_H

#include "ByteBuffer.h"
#include "PacketTypes.h"

struct Payload2
{
    byte id;
    byte dstId;
    Mode mode;
    float lat, lon, alt;
    float angle1, angle2;
    bool isTrue;

    void serialize(ByteBuffer& buf) const;
    void deserialize(ByteBuffer& buf);
};

#endif // PAYLOAD2_H
