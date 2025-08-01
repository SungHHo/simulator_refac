#include "communication/MfrLcCommunicationManager.h"
#include "parsing/MfrParsing.h"

#include <chrono>
#include <thread>
#include <iostream>

class MfrApp : public IReceiver {
private:
    Mfr parser;

public:
    void callBackData(const std::vector<char>& packet) override {
        std::vector<byte> bytePacket(packet.begin(), packet.end());
        parser.parseIncomingData(bytePacket);
    }
};

int main() {
    std::cout << "[MFR] 시스템 시작" << std::endl;

    MfrApp app;
    MfrLcCommManager commManager(&app);

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
