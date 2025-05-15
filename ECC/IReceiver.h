#pragma once

// 인터페이스
class IReceiver
{
public:
    virtual void receive(int len, const char* packet) = 0;
 
};
