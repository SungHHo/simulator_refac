#pragma once
#include "info.h"

#pragma pack(push, 1)

struct Pos3D {
    long long x;
    long long y;
    long long z;
};

struct LSStatus {
    unsigned int id;
    Pos3D position;
    double angle;
    int speed;
    OperationMode mode;
};

#pragma pack(pop)