
// SystemStatus.h
#pragma once
#include <string>
#include <vector>

struct Pos2D
{
    long long x = 0;
    long long y = 0;

    bool isValid() const
    {
        return !(x == 0 && y == 0);
    }
};

enum class MFRMode : unsigned int
{
    STOP = 0,
    ROTATE = 1
};
enum class LauncherMode : unsigned int
{
    STOP = 0,
    MOVE = 1,
    WAR = 2
};

struct MFRStatus
{
    unsigned int mfrId = 0;
    MFRMode mode = MFRMode::STOP;
    double degree = 0.0;
    Pos2D position;
};

struct LSStatus
{
    unsigned int launchSystemId = 0;
    LauncherMode mode = LauncherMode::STOP;
    double launchAngle = 0.0;
    Pos2D position;
};

struct LCStatus
{
    unsigned int LCId = 0;
    Pos2D position;
};

struct MissileStatus
{
    unsigned int id; 
    long long posX;
    long long posY;
    long long altitude;
    int speed;
    double angle;
    unsigned long long detectTime;
    unsigned long long interceptTime;
    bool hit;
};

struct TargetStatus
{
    unsigned int id;
    long long posX;
    long long posY;
    long long altitude;
    int speed;
    double angle;
    unsigned long long detectTime;
    uint8_t priority;
    bool hit;
};

struct SystemStatus
{
    MFRStatus mfr;
    LSStatus ls;
    LCStatus lc;
    std::vector<MissileStatus> missiles;
    std::vector<TargetStatus> targets;
};
