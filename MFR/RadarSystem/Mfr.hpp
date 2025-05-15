#pragma once

#include "MockTarget.hpp"
#include "MockMissile.hpp"
#include "StepMotorController.hpp"
#include "MfrLcCommManager.hpp"
#include "MfrSimCommManager.hpp"
#include "IReceiver.hpp"
#include "PacketProtocol.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>



class Mfr : public IReceiver {
private:
    unsigned int mfrId;
    float mfrX;
    float mfrY;
    int mfrMode;
    double targetMotorAngle;

    StepMotorController stepMotorManager;
    MfrLcCommManager lcCommManager;
    MfrSimCommManager simCommManager;

    std::vector<MockTarget> mockTargets;
    std::vector<MockMissile> mockMissile;

public:
    Mfr();
    
    void startTcp();
    void startUdp();

    void sendToLc(const std::vector<char>& packet);
    std::vector<MockTarget*> MfrDetectionAlgo();
    void controlMotor(int mode, double targetAngle);
    void recvData(const std::vector<char>& packet) override;

    void handleSimDataPayload(const std::vector<char>& packet);

    void addMockTarget(const MockTarget& target);
    MockTarget* getMockTargetById(unsigned int id);
    void removeMockTargetById(unsigned int id);
    void clearMockTargets();

    void addMockMissile(const MockMissile& missile);
    MockMissile* getMockMissileById(unsigned int id);
    void removeMockMissileById(unsigned int id);
    void clearMockMissiles();
};