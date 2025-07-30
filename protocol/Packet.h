#ifndef PACKET_H
#define PACKET_H

#include "ByteBuffer.h"
#include "Header1.h"
#include "Header2.h"
#include "Payload2.h"
#include "PacketTypes.h"

class Packet
{
public:
    Header1 h1;
    Header2 h2;
    Payload2 payload2;

    void serialize(ByteBuffer& buf) const;
    void deserialize(ByteBuffer& buf);
};

#endif // PACKET_H
