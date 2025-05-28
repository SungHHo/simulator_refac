#pragma once
#include <string>

struct SimulatorCommConfig {
    std::string ip;
    int port;
};

struct FireControlCommConfig {
    std::string devicePath;
    int baudRate;
    int dataBits;
    int stopBits;
    char parity;
};

enum class LauncherMode { STOP_MODE, MOVE_MODE, WAR_MODE };

struct LauncherConfig {
    unsigned int id;
    long long posX;
    long long posY;
    double launchAngle;
    int launchSpeed;
    LauncherMode mode;
};

struct Config {
    SimulatorCommConfig simulator;
    FireControlCommConfig fireControl;
    LauncherConfig launcher;
};
