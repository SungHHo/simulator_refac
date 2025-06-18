#include <stdexcept>
#include "LCToLSCommFactory.hpp"
#include "ConfigParser.hpp"
#include "LCToLSCommManager.hpp"
#include "LCToLSCommUDPManager.hpp"

std::unique_ptr<LCToLSCommInterface> createLCToLSComm(
    SerialReceiverInterface& receiver, 
    const std::string& configPath)
{
    std::string type = ConfigParser::getValue("FireControlComm", "Type", configPath);

    if (type == "UART") 
    {
        return std::make_unique<LCToLSCommManager>(receiver, configPath);
    }
    else if (type == "UDP") 
    {
        return std::make_unique<LCToLSUDPCommManager>(receiver, configPath);
    }
    else 
    {
        throw std::runtime_error("[LCToLSCommFactory] Unknown communication type: " + type);
    }
}
