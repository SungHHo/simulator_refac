#include "Header2.h"

void Header2::serialize(ByteBuffer& buf) const
{
    buf.writeByte(srcId);
    buf.writeByte(dstId);
    buf.writeByte((byte)msgType);
    buf.writeUInt32(payloadLength);
    buf.writeUInt32(seq);
    buf.writeUInt32(count);
    buf.writeInt32(time1);
    buf.writeInt32(time2);
}

void Header2::deserialize(ByteBuffer& buf)
{
    srcId = buf.readByte();
    dstId = buf.readByte();
    msgType = (MsgType)buf.readByte();
    payloadLength = buf.readUInt32();
    seq = buf.readUInt32();
    count = buf.readUInt32();
    time1 = buf.readInt32();
    time2 = buf.readInt32();
}
