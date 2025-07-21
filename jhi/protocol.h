#ifndef PROTOCOL_H
#define PROTOCOL_H

#pragma pack(push, 1)

enum MsgType
{
    SYSTEM_INFO = 0,
    LAUNCH_INFO = 1
};

enum Mode
{
    ROTAION_MODE = 0, 
    STOP_MODE = 1
};

struct PrimaryHeader
{
    char srcID;
    char destID;
    char lcID;
    unsigned int payloadLen;
    int time1;
    int time2;
};

struct SecondaryHeader
{
    char srcID;
    char destID;
    char msgType;
    unsigned int length;
    unsigned int seq;
    unsigned int count;
    int time1;
    int time2;
};

struct SecondaryPayload
{
    char ID;
    char destID;
    char mode;
    float lat;
    float lon;
    float alt;
    float angle1;
    float angle2;
    char isTrue;
};

#pragma pack(pop)

#endif