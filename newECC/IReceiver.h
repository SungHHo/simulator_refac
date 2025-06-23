#pragma once

class IReceiver {
public:
    virtual ~IReceiver() = default;

    // ���ŵ� raw ������ ����
    virtual void receive(int len, const char* packet) = 0;
};
