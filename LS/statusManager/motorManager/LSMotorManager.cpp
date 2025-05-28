#include "LSMotorManager.hpp"
#include <cmath>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>

LSMotorManager::LSMotorManager(const std::string& canInterface, uint32_t id)
    : canId(id)
{
    initCAN(canInterface);
}

LSMotorManager::~LSMotorManager()
{
    if (canSocket >= 0)
        close(canSocket);
}

void LSMotorManager::initCAN(const std::string& canInterface)
{
    canSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (canSocket < 0)
    {
        perror("socket");
        throw std::runtime_error("CAN socket creation failed");
    }

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, canInterface.c_str(), IFNAMSIZ - 1);
    if (ioctl(canSocket, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("ioctl");
        throw std::runtime_error("CAN interface error");
    }

    struct sockaddr_can addr {};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(canSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        throw std::runtime_error("CAN bind error");
    }

    std::cout << "[LSMotorManager] CAN socket opened on " << canInterface << ", ID=0x" 
              << std::hex << canId << std::dec << "\n";
}

std::pair<bool, double> LSMotorManager::computeShortestRotation(double current, double target)
{
    current = std::fmod(current, 360.0);
    if (current < 0) current += 360.0;

    target = std::fmod(target, 360.0);
    if (target < 0) target += 360.0;

    double diff = target - current;
    if (diff > 180.0) diff -= 360.0;
    else if (diff < -180.0) diff += 360.0;

    bool clockwise = diff < 0;
    double degrees = std::abs(diff);

    return {clockwise, degrees};
}

void LSMotorManager::rotateToAngle(double currentAngle, double targetAngle)
{
    auto [clockwise, degrees] = computeShortestRotation(currentAngle, targetAngle);

    uint16_t speedRPM = 320;
    uint8_t acceleration = 2;

    uint32_t pulses = static_cast<uint32_t>(degrees / DEGREE_PER_STEP);

    auto frame = buildPositionModeFrame(clockwise, speedRPM, acceleration, pulses);
    sendCanFrame(frame);

    std::cout << "[LSMotorManager] rotateToAngle(): current=" << currentAngle
              << "°, target=" << targetAngle << "°, CW=" << clockwise
              << ", pulses=" << pulses << "\n";
}

std::vector<uint8_t> LSMotorManager::buildPositionModeFrame(bool clockwise, uint16_t speed, uint8_t acc, uint32_t pulses)
{
    uint8_t dir_bit = clockwise ? 0x00 : 0x80; // b7 = direction (0=CW, 1=CCW)
    uint8_t speed_high = dir_bit | ((speed >> 4) & 0x7F); // 7-bit speed high with direction
    uint8_t speed_low = (speed & 0x0F) << 4;              // 4-bit speed low shifted

    return {
        0xFD,                      // code
        0x01,                      // Rev (position mode)
        speed_high,               // byte2
        speed_low,                // byte3
        acc,                      // byte4
        static_cast<uint8_t>(pulses >> 16), // byte5
        static_cast<uint8_t>(pulses >> 8),  // byte6
        static_cast<uint8_t>(pulses)        // byte7
    };
}

void LSMotorManager::sendCanFrame(const std::vector<uint8_t>& data)
{
    struct can_frame frame {};
    frame.can_id = canId;
    frame.can_dlc = data.size();
    std::memcpy(frame.data, data.data(), data.size());

    if (write(canSocket, &frame, sizeof(frame)) != sizeof(frame))
    {
        perror("write(CAN)");
    }
}

bool LSMotorManager::receiveStatus()
{
    struct can_frame frame {};
    int nbytes = read(canSocket, &frame, sizeof(frame));

    if (nbytes < 0)
    {
        perror("read(CAN)");
        return false;
    }

    if (frame.can_id != canId || frame.can_dlc < 3 || frame.data[0] != 0xFD)
        return false;

    uint8_t status = frame.data[1];

    if (status == 2) {
        std::cout << "[LSMotorManager] rotation complete (status=2)\n";
        return true;
    } else if (status == 0) {
        std::cout << "[LSMotorManager] rotation failed (status=0)\n";
        return false;
    }

    return false;
}
