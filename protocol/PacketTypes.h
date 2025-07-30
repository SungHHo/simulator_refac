#ifndef PACKET_TYPES_H
#define PACKET_TYPES_H

#include <stdint.h>

// 바이트 단위 표현을 위한 별칭
typedef uint8_t byte;

// 메시지 타입
enum MsgType
{
    MSG_TYPE_COMMAND = 0,
    MSG_TYPE_STATUS = 1,
};

// 발사대, 레이더 동작 모드
enum Mode
{
    // 발사대: 정지 모드, 레이더: 정지 모드
    MODE_STOP = 0,
    // 발사대: 이동 모드, 레이더: 회전 모드
    MODE_MOVE = 1,
    // 발사대: 교전 모드
    MODE_ENGAGE = 2
};

// 최대 크기 등의 상수 정의
#define MAX_PAYLOAD_SIZE 512
#define PACKET_HEADER_SIZE 16

#endif // PACKET_TYPES_H
