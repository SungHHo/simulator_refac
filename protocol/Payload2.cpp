#include "Payload2.h"

void Payload2::serialize(ByteBuffer& buf) const
{
    buf.writeByte(id);
    buf.writeByte(dstId);
    buf.writeByte((byte)mode);
    buf.writeFloat(lat);
    buf.writeFloat(lon);
    buf.writeFloat(alt);
    buf.writeFloat(angle1);
    buf.writeFloat(angle2);
    buf.writeByte(isTrue ? 1 : 0);
}

void Payload2::deserialize(ByteBuffer& buf)
{
    id = buf.readByte();
    dstId = buf.readByte();
    mode = (Mode)buf.readByte();
    lat = buf.readFloat();
    lon = buf.readFloat();
    alt = buf.readFloat();
    angle1 = buf.readFloat();
    angle2 = buf.readFloat();
    isTrue = (buf.readByte() != 0);
}
