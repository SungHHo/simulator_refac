#include <vector>
#include <cstring>
#include <stdint.h>
#include "PacketTypes.h"

typedef uint8_t byte;

class ByteBuffer 
{
    std::vector<byte> buffer;
    size_t offset;

public:
    // 생성자 (오프셋은 0으로 초기화)
    ByteBuffer() : offset(0) {}

    // 버퍼에 저장된 데이터 반환
    const std::vector<byte>& data() const { return buffer; }

    // 버퍼의 크기 반환
    size_t size() const { return buffer.size(); }

    // offset 설정 (초기화)
    void resetOffset() { offset = 0; }
    
    // 버퍼 설정
    void setBuffer(const std::vector<byte>& buf) {
        buffer = buf;
        offset = 0;
    }

// ---------------------------------- 버퍼 쓰기 메소드 -------------------------------------
    // 1바이트 저장
    void writeByte(byte b) { buffer.push_back(b); }
    // int 형 데이터 저장
    // LSB부터 차례대로 낮은 주소부터 저장 -> 리틀 엔디안 방식
    void writeInt32(int32_t val)
    {
        for (int i = 0; i < 4; ++i)
            buffer.push_back((val >> (i * 8)) & 0xFF);
    }
    // unsigned int 형 데이터 저장
    // LSB부터 차례대로 낮은 주소부터 저장 -> 리틀 엔디안 방식
    void writeUInt32(uint32_t val)
    {
        for (int i = 0; i < 4; ++i)
            buffer.push_back((val >> (i * 8)) & 0xFF);
    }
    // float 형 데이터 저장
    // LSB부터 차례대로 낮은 주소부터 저장 -> 리틀 엔디안 방식
    void writeFloat(float f)
    {
        // float 데이터는 비트 연산이 불가하므로 byte단위로 형 변환 후 저장
        byte* p = reinterpret_cast<byte*>(&f);
        for (int i = 0; i < 4; ++i)
            buffer.push_back(p[i]);
    }
// ------------------------------------------------------------------------------------



// ---------------------------------- 버퍼 읽기 메소드 ----------------------------------
    // 버퍼의 내용 1바이트 읽기 (읽은 후 offset 1 증가)
    byte readByte() { return buffer[offset++]; }
    // 버퍼의 내용 4바이트(int형 정수) 읽기 (읽은 후 offset 4 증가)
    int32_t readInt32()
    {
        int32_t val = 0;
        for (int i = 0; i < 4; ++i)
            val |= buffer[offset++] << (i * 8);
        return val;
    }
    // 버퍼의 내용 4바이트(unsigned int형 정수) 읽기 (읽은 후 offset 4 증가)
    uint32_t readUInt32()
    {
        uint32_t val = 0;
        for (int i = 0; i < 4; ++i)
            val |= buffer[offset++] << (i * 8);
        return val;
    }
    // 버퍼의 내용 4바이트(float형 데이터) 읽기 (읽은 후 offset 4 증가)
    float readFloat()
    {
        float f;
        // 4 바이트 f에 저장
        std::memcpy(&f, &buffer[offset], 4);
        offset += 4;
        return f;
    }
// ------------------------------------------------------------------------------------

};
