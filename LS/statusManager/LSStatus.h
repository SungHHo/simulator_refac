#pragma once
#include "info.hpp"

#pragma pack(push, 1)

struct Pos2D {
    long long x;
    long long y;
};

struct LSStatus {
    unsigned int id;
    Pos2D position;
    double angle;
    int speed;
    OperationMode mode;
};

#pragma pack(pop)