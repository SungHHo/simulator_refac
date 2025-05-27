#pragma once
#include <vector>

class IReceiver
{
public:
    virtual void recvData(const std::vector<char>& packet) = 0;
    virtual ~IReceiver() = default;
};
