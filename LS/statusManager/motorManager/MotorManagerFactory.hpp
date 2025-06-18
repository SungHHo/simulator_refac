#pragma once
#include "MotorManagerInterface.hpp"
#include "LSMotorManager.hpp"
#include "TestMotorManager.hpp"
#include <memory>
#include <string>

class MotorManagerFactory {
public:
    static std::unique_ptr<MotorManagerInterface> create(const std::string& type,
                                                         const std::string& canInterface,
                                                         uint32_t canId) {
        if (type == "REAL") {
            return std::make_unique<LSMotorManager>(canInterface, canId);
        } else if (type == "TEST") {
            return std::make_unique<TestMotorManager>();
        } else {
            throw std::invalid_argument("Unknown MotorManager type: " + type);
        }
    }
};