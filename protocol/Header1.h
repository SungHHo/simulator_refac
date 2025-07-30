#ifndef HEADER1_H
#define HEADER1_H

#include "ByteBuffer.h"

struct Header1 
{
    byte srcID;         // 최초 송신 장비 ID
    byte destID;        // 최종 수신 장비 ID
    byte LC_ID;         // 발사통제기 ID
    uint32_t payloadLength;
    int32_t time1;
    int32_t time2;

    void serialize(ByteBuffer& buf) const;
    void deserialize(ByteBuffer& buf);
};

#endif // HEADER1_H
