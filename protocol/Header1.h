#include "ByteBuffer.h"

// | 송신 ID (1바이트) | 수신 ID (1바이트) | 발사통재기 ID (1바이트) | 페이로드 길이 (4바이트) | 송신 시간 (4바이트) | 수신 시간 (4바이트) |
// total = 15바이트
struct Header1 
{
    // 최초 송신 장비 ID
    byte srcID;
    // 최종 수신 장비 ID
    byte destID;
    // 발사통제기 ID => 정합성 체크
    byte LC_ID;
    // 페이로드 길이 
    uint32_t payloadLength;
    // 송신 시간
    int32_t time1;
    // 수신 시간
    int32_t time2;

    // 버퍼에 데이터 삽입
    void serialize(ByteBuffer& buf) const
    {
        buf.writeByte(srcID);
        buf.writeByte(destID);
        buf.writeByte(LC_ID);
        buf.writeUInt32(payloadLength);
        buf.writeInt32(time1);
        buf.writeInt32(time2);
    }

    // 버퍼에서 데이터 추출
    void deserialize(ByteBuffer& buf)
    {
        srcID = buf.readByte();
        destID = buf.readByte();
        LC_ID = buf.readByte();
        payloadLength = buf.readUInt32();
        time1 = buf.readInt32();
        time2 = buf.readInt32();
    }
};

