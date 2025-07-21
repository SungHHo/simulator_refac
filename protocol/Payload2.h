#include "ByteBuffer.h"

struct Payload2
{
    // 페이로드 id
    char id;
    // 도착지 id
    char dstId;
    // 모드 (레이더, 발사대)
    Mode mode;
    // 위도, 경도, 고도 
    float lat, lon, alt;
    // 방위각, 고도각
    float angle1, angle2;
    // 격추 여부
    bool isTrue;

    // 버퍼에 데이터 삽입
    void serialize(ByteBuffer& buf) const
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

    // 버퍼에서 데이터 추출
    void deserialize(ByteBuffer& buf)
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
};
