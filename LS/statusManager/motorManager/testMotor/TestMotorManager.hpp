#pragma once
#include "MotorManagerInterface.hpp"
#include <iostream>

class TestMotorManager : public MotorManagerInterface {
public:
    bool rotateToAngle(double currentAngle, double targetAngle) override {
        std::cout << "[TestMotorManager] Simulated rotation from " << currentAngle
                  << " to " << targetAngle << " completed.\n";
        return true;
    }
};
