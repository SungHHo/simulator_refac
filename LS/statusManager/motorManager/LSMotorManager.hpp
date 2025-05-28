#pragma once
#include <cstdint>
#include <vector>
#include <string>

class LSMotorManager
{
private:
    int canSocket;
    uint32_t canId;

    static constexpr double DEGREE_PER_STEP = 0.1125; // 예: 3200펄스/회전이면 360/3200

    void initCAN(const std::string& canInterface);
    std::pair<bool, double> computeShortestRotation(double current, double target);

    std::vector<uint8_t> buildPositionModeFrame(bool clockwise, uint16_t speed, uint8_t acc, uint32_t pulses);
    void sendCanFrame(const std::vector<uint8_t>& frame);

public:
    LSMotorManager(const std::string& canInterface, uint32_t canId);
    ~LSMotorManager();

    void rotateToAngle(double currentAngle, double targetAngle);
    bool receiveStatus(); // ← 상태 확인용
};
