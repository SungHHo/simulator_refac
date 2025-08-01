#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <vector>
#include <stdint.h>

typedef uint8_t byte;

class ByteBuffer {
    std::vector<byte> buffer;
    size_t offset;

public:
    ByteBuffer();

    const std::vector<byte>& data() const;
    size_t size() const;
    void resetOffset();
    void setBuffer(const std::vector<byte>& buf);

    void writeByte(byte b);
    void writeInt32(int32_t val);
    void writeUInt32(uint32_t val);
    void writeFloat(float f);

    byte readByte();
    int32_t readInt32();
    uint32_t readUInt32();
    float readFloat();
};

#endif
