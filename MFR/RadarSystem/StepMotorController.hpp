#pragma once

class StepMotorController {
public:
    float targetRps;
    int stepsPerRev;
    float stepsPerAngle;
    int microStep;
    int pulsePerRev;
    float pulseIntervalUs;
    float totalPulse;

    void runSpeedMode() 
    {

    }

    void runAngleMode(double angle) 
    {

    }
};
