#include "ByteBuffer.h"

// | 송신 ID (1바이트) | 수신 ID (1바이트) | 메시지 타입 (1바이트) | 페이로드 길이 (4바이트) | 패킷 순서 (4바이트) | 객체 개수 (4바이트) | 송신 시간 (4바이트) | 수신 시간 (4바이트) |
// total = 23바이트
struct Header2
{
    // 최초 송신 장비 ID
    byte srcId;
    // 최종 수신 장비 ID
    byte dstId;
    // 메세지 타입 (명령 / 상태 전송)
    MsgType msgType;
    // 페이로드 길이
    uint32_t payloadLength;
    // 패킷 순서
    uint32_t seq;
    // 객체 개수
    uint32_t count;
    // 송신 시간
    int32_t time1;
    // 수신 시간
    int32_t time2;

    // 버퍼에 데이터 삽입
    void serialize(ByteBuffer& buf) const
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

    // 버퍼에서 데이터 추출
    void deserialize(ByteBuffer& buf)
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
};
