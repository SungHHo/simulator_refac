#include "Header1.h"

void Header1::serialize(ByteBuffer& buf) const
{
    buf.writeByte(srcID);
    buf.writeByte(destID);
    buf.writeByte(LC_ID);
    buf.writeUInt32(payloadLength);
    buf.writeInt32(time1);
    buf.writeInt32(time2);
}

void Header1::deserialize(ByteBuffer& buf)
{
    srcID = buf.readByte();
    destID = buf.readByte();
    LC_ID = buf.readByte();
    payloadLength = buf.readUInt32();
    time1 = buf.readInt32();
    time2 = buf.readInt32();
}
