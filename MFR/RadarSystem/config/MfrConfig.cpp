#include "MfrConfig.h"
#include "ini.h"
#include <string>
#include <iostream>

int iniHandler(void* user, const char* section, const char* name, const char* value)
{
    MfrConfig* config = reinterpret_cast<MfrConfig*>(user);

    std::string s(section), n(name), v(value);

    if (s == "LaunchController")
    {
        if (n == "IP")
        {
            config->launchControllerIP = v;
        }
        else if(n == "Port")
        {
            config->launchControllerPort = std::stoi(v);
        }
    }

    else if (s == "Simulator")
    {
        if (n == "Port")
        {
            config->simulatorPort = std::stoi(v);
        }
    }

    return 1;
}

bool loadMfrConfig(const std::string& path, MfrConfig& config) 
{
    if (ini_parse(path.c_str(), iniHandler, &config) < 0)
    {
        std::cerr << "[loadMfrConfig] INI 파일 읽기 실패: " << path << std::endl;
        return false;
    }
    return true;
}
