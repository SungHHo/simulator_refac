#include "Packet.h"

void Packet::serialize(ByteBuffer& buf) const
{
    h1.serialize(buf);
    h2.serialize(buf);
    payload2.serialize(buf);
}

void Packet::deserialize(ByteBuffer& buf)
{
    h1.deserialize(buf);
    h2.deserialize(buf);

    switch (h2.msgType)
    {
        case MSG_TYPE_COMMAND:
        case MSG_TYPE_STATUS:
            payload2.deserialize(buf);
            break;

        default:
            // 예외 처리 또는 무시
            break;
    }
}
