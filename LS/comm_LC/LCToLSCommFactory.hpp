#pragma once

#include <memory>
#include <string>
#include "LCToLSCommInterface.hpp"
#include "SerialReceiverInterface.hpp"

std::unique_ptr<LCToLSCommInterface> createLCToLSComm(
    SerialReceiverInterface& receiver, 
    const std::string& configPath);
