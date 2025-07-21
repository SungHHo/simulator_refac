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

    void serialize(ByteBuffer& buf) const
    {
        h1.serialize(buf);
        h2.serialize(buf);
        payload2.serialize(buf);
    }

    void deserialize(ByteBuffer& buf)
    {
        h1.deserialize(buf);
        h2.deserialize(buf);

        // 추후 메시지 타입에 따라 분기 가능
        switch (h2.msgType)
        {
            case MSG_TYPE_COMMAND:
            case MSG_TYPE_STATUS:
                payload2.deserialize(buf);
                break;

            default:
                // 에러 처리
                break;
        }
    }
};

#endif // PACKET_H

