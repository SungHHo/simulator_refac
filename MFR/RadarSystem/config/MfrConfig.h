#ifndef MFR_CONFIG_H
#define MFR_CONFIG_H

#include <string>

struct MfrConfig {
    std::string launchControllerIP;
    int launchControllerPort = 0;
    int simulatorPort = 0;
};

int iniHandler(void* user, const char* section, const char* name, const char* value);
bool loadMfrConfig(const std::string& path, MfrConfig& config);  // 선언

#endif // MFR_CONFIG_H
