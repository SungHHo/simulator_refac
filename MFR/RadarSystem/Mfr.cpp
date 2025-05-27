#include "Mfr.hpp"

Mfr::Mfr()
    : mfrId(1), mfrX(10), mfrY(10), mfrMode(0), targetMotorAngle(0.0),
      lcCommManager(this), 
      simCommManager(this)
{}

void Mfr::startUdp()
{
    std::cout << "[MFR] UDP 통신 수신 시작" << "\n";
    simCommManager.startUdpReceiver();
}

void Mfr::startTcp()
{
    std::cout << "[MFR] TCP 통신 수신 시작" << "\n";
    if (lcCommManager.connectToLc()) 
    {
        lcCommManager.startTcpReceiver();
    }
}

//
//  표적 탐지 알고리즘 관련
//
std::vector<MockTarget*> Mfr::MfrDetectionAlgo()
{
    const double detectionRange = 100.0;
    std::vector<MockTarget*> detectedTargets;
    
    for (auto& target : mockTargets) 
    {
        long long dx = target.targetX- mfrX;
        long long dy = target.targetY - mfrY;
        double distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance <= detectionRange) 
        {
            detectedTargets.push_back(&target);
        }
    }

    return detectedTargets;
}

//
//  통신 관련
//
void Mfr::sendToLc(const std::vector<char>& packet) 
{
    std::cout << "[Mfr] sendToLc 호출됨, size: " << packet.size() << std::endl;
}

void Mfr::recvData(const std::vector<char>& packet)
{
    if (packet.size() < 1) 
    {
        std::cerr << "[MFR] 이상 패킷" << std::endl;
        return;
    }

    uint8_t cmd = static_cast<uint8_t>(packet[0]);

    std::vector<char> payloadOnly(packet.begin() + 1, packet.end());

    switch (cmd) 
    {
        case SIM_TARGET_DATA:
            handleSimDataPayload(payloadOnly);
            break;

        case SIM_MISSILE_DATA:
            handleSimDataPayload(payloadOnly);
            break;

        case STATUS_REQ:
            std::cout << "[MFR] 상태 요청 명령 수신됨" << std::endl;
            
            break;

        default:
            std::cerr << "[MFR] 미정의 명령어 타입: 0x"
                      << std::hex << static_cast<int>(cmd) << std::dec << std::endl;
            break;
    }
}


//
//  모의 객체 관리 관련
//
void Mfr::handleSimDataPayload(const std::vector<char>& payload)
{
    if (payload.size() != sizeof(SimData)) 
    {
        std::cerr << "[MFR] SimData 크기 오류. 받은 크기: " << payload.size() << std::endl;
        return;
    }
    
    SimData data;
    std::memcpy(&data, payload.data(), sizeof(SimData));

    std::cout << "[MFR] SimData 수신 → ID: " << data.mockId
              << ", X: " << data.x
              << ", Y: " << data.y
              << ", Z: " << data.z
              << ", Angle: " << data.angle
              << ", Speed: " << data.speed << std::endl;

    if (data.mockId >= 101001 && data.mockId <= 101999) 
    {
        MockTarget target{data.mockId, data.x, data.y, static_cast<double>(data.angle), data.speed};
        addMockTarget(target);
        std::cout << "[MFR] → 모의 표적 등록 완료" << std::endl;
    }
    else if (data.mockId >= 102001 && data.mockId <= 102999) 
    {
        MockMissile missile{data.mockId, data.x, data.y, data.z, static_cast<double>(data.angle), data.speed};
        addMockMissile(missile);
        std::cout << "[MFR] → 모의 미사일 등록 완료" << std::endl;
    }
    else 
    {
        std::cerr << "[MFR] 알 수 없는 ID 범위: " << data.mockId << std::endl;
    }
}

void Mfr::addMockTarget(const MockTarget& target) 
{
    mockTargets.push_back(target);
}

MockTarget* Mfr::getMockTargetById(unsigned int id) 
{
    for (auto& t : mockTargets) 
    {
        if (t.id == id)
        {
            return &t;
        }
    }
    return nullptr;
}

void Mfr::removeMockTargetById(unsigned int id) 
{
    mockTargets.erase(
        std::remove_if(mockTargets.begin(), mockTargets.end(),
            [id](const MockTarget& t) { return t.id == id; }),
        mockTargets.end());
}

void Mfr::clearMockTargets() 
{
    mockTargets.clear();
}

void Mfr::addMockMissile(const MockMissile& missile) 
{
    mockMissile.push_back(missile);
}

MockMissile* Mfr::getMockMissileById(unsigned int id) 
{
    for (auto& m : mockMissile) 
    {
        if (m.id == id)
        {
            return &m;
        }
    }
    return nullptr;
}

void Mfr::removeMockMissileById(unsigned int id) 
{
    mockMissile.erase(
        std::remove_if(mockMissile.begin(), mockMissile.end(),
            [id](const MockMissile& m) { return m.id == id; }),
        mockMissile.end());
}

void Mfr::clearMockMissiles() 
{
    mockMissile.clear();
}

//
//  모터 제어 관련
//
void Mfr::controlMotor(int mode, double targetAngle) 
{
    stepMotorManager.runAngleMode(targetAngle);
}