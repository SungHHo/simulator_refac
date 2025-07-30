#ifndef HEADER2_H
#define HEADER2_H

#include "ByteBuffer.h"
#include "PacketTypes.h"

struct Header2
{
    byte srcId;
    byte dstId;
    MsgType msgType;
    uint32_t payloadLength;
    uint32_t seq;
    uint32_t count;
    int32_t time1;
    int32_t time2;

    void serialize(ByteBuffer& buf) const;
    void deserialize(ByteBuffer& buf);
};

#endif // HEADER2_H
