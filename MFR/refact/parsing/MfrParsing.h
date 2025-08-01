#ifndef MFR_PARSING_H
#define MFR_PARSING_H

#include <vector>
#include "Packet.h"

class Mfr
{
public:
    bool parseIncomingData(const std::vector<byte>& rawData);

private:
    void handleCommand(const Packet& pkt);
    void handleStatus(const Packet& pkt);
};

#endif // MFR_PARSING_H
