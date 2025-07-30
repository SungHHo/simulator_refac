#include "ByteBuffer.h"
#include <cstring>

// 생성자
ByteBuffer::ByteBuffer() : offset(0) {}

const std::vector<byte>& ByteBuffer::data() const { return buffer; }
size_t ByteBuffer::size() const { return buffer.size(); }
void ByteBuffer::resetOffset() { offset = 0; }

void ByteBuffer::setBuffer(const std::vector<byte>& buf)
{
    buffer = buf;
    offset = 0;
}

void ByteBuffer::writeByte(byte b)
{
    buffer.push_back(b);
}

void ByteBuffer::writeInt32(int32_t val)
{
    for (int i = 0; i < 4; ++i)
        buffer.push_back((val >> (i * 8)) & 0xFF);
}

void ByteBuffer::writeUInt32(uint32_t val)
{
    for (int i = 0; i < 4; ++i)
        buffer.push_back((val >> (i * 8)) & 0xFF);
}

void ByteBuffer::writeFloat(float f)
{
    byte* p = reinterpret_cast<byte*>(&f);
    for (int i = 0; i < 4; ++i)
        buffer.push_back(p[i]);
}

byte ByteBuffer::readByte()
{
    return buffer[offset++];
}

int32_t ByteBuffer::readInt32()
{
    int32_t val = 0;
    for (int i = 0; i < 4; ++i)
        val |= buffer[offset++] << (i * 8);
    return val;
}

uint32_t ByteBuffer::readUInt32()
{
    uint32_t val = 0;
    for (int i = 0; i < 4; ++i)
        val |= buffer[offset++] << (i * 8);
    return val;
}

float ByteBuffer::readFloat()
{
    float f;
    std::memcpy(&f, &buffer[offset], 4);
    offset += 4;
    return f;
}