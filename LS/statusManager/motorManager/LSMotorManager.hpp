#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "MotorManagerInterface.hpp"

class LSMotorManager :  public MotorManagerInterface
{
private:
    int canSocket;
    uint32_t canId;

    static constexpr double DEGREE_PER_STEP = 0.1125; // 1.8도 / 16분해능

    void initCAN(const std::string& canInterface);
    std::pair<bool, double> computeShortestRotation(double current, double target);

    std::vector<uint8_t> buildPositionModeFrame(bool clockwise, uint16_t speed, uint8_t acc, uint32_t pulses);
    void sendCanFrame(const std::vector<uint8_t>& frame);
    bool receiveStatus(); // 회전 완료 상태 수신

public:
    LSMotorManager(const std::string& canInterface, uint32_t canId);
    ~LSMotorManager();

    bool rotateToAngle(double currentAngle, double targetAngle);
};
