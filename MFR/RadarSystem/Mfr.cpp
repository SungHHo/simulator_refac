#include "Mfr.hpp"

Mfr::Mfr() : goalTargetId(-1), mfrMode(ROTATION_MODE), goalMotorAngle(135.0)
{
    stepMotorManager = new StepMotorController(); // 매개변수 없는 기본 생성자 사용
    // lcCommManager = new MfrLcCommManager(this);   // this는 IReceiver*로 사용됨
    simCommManager = new MfrSimCommManager(this); // 동일하게 this 전달
    startDetectionAlgoThread();
    // requestLcInitData();
}

Mfr::~Mfr()
{
    delete stepMotorManager;
    delete lcCommManager;
    delete simCommManager;
}

void Mfr::callBackData(const std::vector<char>& packet)
{
    if (packet.size() < 1) 
    {
        std::cerr << "[Mfr::recvData] 이상 패킷" << std::endl;
        return;
    }

    uint8_t cmd = static_cast<uint8_t>(packet[0]);

    std::vector<char> dataPayload(packet.begin() + 1, packet.end());
    switch (cmd) 
    {
        case SIM_MOCK_DATA:
            parsingSimData(dataPayload);
            break;

        case STATUS_REQ:
            std::cout << "[Mfr::recvData] 상태 요청 명령 수신됨" << std::endl;
            sendMfrStatus();
            std::cout << "[Mfr::recvData] 상태 요청 명령 전송됨" << std::endl;
            break;

        case MODE_CHANGE:
            std::cout << "[Mfr::recvData] 모드 변경 명령 수신됨" << std::endl; 
            parsingModeChangeData(dataPayload);
            break;

        case LC_INIT_RES:
            std::cout << "[Mfr::recvData] LC 최초 정보 수신" << std::endl;
            parsingLcInitData(dataPayload);
            break;

        default:
            std::cerr << "[Mfr::recvData] 미정의 명령어 타입: 0x"
                      << std::hex << static_cast<int>(cmd) << std::dec << std::endl;
            break;
    }
}

template <typename T>
std::vector<char> Mfr::serializePacketforSend(const T& status)
{
    std::vector<char> buffer(sizeof(T));
    std::memcpy(buffer.data(), &status, sizeof(T));
    return buffer;
}

void Mfr::requestLcInitData()
{
    ReqLcInitData reqData{};
    reqData.radarId = mfrId;

    std::vector<char> packet = serializePacketforSend(reqData);
    
    packet.insert(packet.begin(), static_cast<char>(LC_INIT_REQ));

    lcCommManager->send(packet);
}

void Mfr::sendMfrStatus()
{
    MfrStatus status{};
    status.radarId = mfrId;
    status.radarPos = { mfrCoords.latitude, mfrCoords.longitude, mfrCoords.altitude };
    status.radarMode = mfrMode;
    status.radarAngle = goalMotorAngle;

    std::vector<char> packet = serializePacketforSend(status);

    packet.insert(packet.begin(), static_cast<char>(STATUS_RES));

    lcCommManager->send(packet);
}

std::vector<char> Mfr::serializeDetectionPacket(const std::vector<MfrToLcTargetInfo>& targets, const std::vector<MfrToLcMissileInfo>& missiles)
{
    std::vector<char> buffer;

    buffer.push_back(static_cast<char>(DETECTED_INFO));
    buffer.push_back(mfrId);
    buffer.push_back(static_cast<unsigned char>(targets.size()));
    buffer.push_back(static_cast<unsigned char>(missiles.size()));

    for (const auto& target : targets)
    {
        size_t offset = buffer.size();
        buffer.resize(offset + sizeof(MfrToLcTargetInfo));
        std::memcpy(buffer.data() + offset, &target, sizeof(MfrToLcTargetInfo));
        //target 출력
        // std::cout << "[Mfr::serializeDetectionPacket] Target ID: " << target.id
        //           << ", Coords: (" << target.targetCoords.x << ", " << target.targetCoords.y << ")"
        //           << ", Speed: " << target.targetSpeed
        //           << ", Angle: " << target.targetAngle
        //           << ", Priority: " << static_cast<int>(target.prioirty) 
        //           << ", First Detection Time: " << target.firstDetectionTime
        //           << ", Is Hit: " << (target.isHit ? "Yes" : "No") 
        //           << std::endl;
    }

    for (const auto& missile : missiles)
    {
        size_t offset = buffer.size();
        buffer.resize(offset + sizeof(MfrToLcMissileInfo));
        //missile 출력
        // std::cout << "[Mfr::serializeDetectionPacket] missile ID: " << missile.id
        //           << ", Coords: (" << missile.missileCoords.latitude << ", " << missile.missileCoords.longitude << ", " << missile.missileCoords.altitude << ")"
        //           << ", Speed: " << missile.missileSpeed
        //           << ", Angle: " << missile.missileAngle
        //           << ", First Detection Time: " << missile.firstDetectionTime
        //           << ", Time to Intercept: " << missile.timeToIntercept
        //           << ", Is Hit: " << (missile.isHit ? "Yes" : "No")
        //           << std::endl;
        std::memcpy(buffer.data() + offset, &missile, sizeof(MfrToLcMissileInfo));
    }

    return buffer;
}

void Mfr::parsingModeChangeData(const std::vector<char>& payload) 
{
    if (payload.size() < 5)
    {
        std::cerr << "[Mfr::parsingModeChangeData] ModeChangeData 크기 오류. 받은 크기: " << payload.size() << std::endl;
        return;
    }

    unsigned char radarId = payload[0];
    unsigned char modeData = payload[1];    

    if(!(radarId == mfrId))
    {
        std::cerr << "[Mfr::parsingModeChangeData] mfrId 미일치. 받은 mfrId: " << payload[0] << std::endl;
        return;
    }

    unsigned int targetId = 0;
    if(modeData == ROTATION_MODE)
    {
        mfrMode = ROTATION_MODE;
        // stepMotorManager->runSpeedMode(motorTargetRPM);
        std::cout << "[Mfr::parsingModeChangeData] 모드 변경: ROTATION_MODE" << std::endl;
    }

    if(modeData == ANGLE_MODE)
    {
        if (payload.size() < 5 + sizeof(unsigned int) + sizeof(unsigned char))
        {
            std::cerr << "[Mfr::parsingModeChangeData] targetId 포함인데도 길이가 부족함" << std::endl;
            return;
        }

        unsigned char isPriorityMode = payload[2];
        if(isPriorityMode == PRIORITY_TARGET)
        {
            // detectedTargets[goalTargetId]
        }

        else if(isPriorityMode == CUSTOMIZE_TARGET)
        {

        }
        // stepMotorManager->runAngleMode(goalMotorAngle);

        std::memcpy(&targetId, &payload[3], sizeof(unsigned int));
        std::cout << "[Mfr::parsingModeChangeData] Priority targetId: " << targetId << std::endl;
        goalTargetId = targetId;
    }
}

void Mfr::parsingLcInitData(const std::vector<char>& payload)
{
    LcInitData status{};
    std::memcpy(&status, payload.data(), sizeof(LcInitData));
    lcCoords.x = status.lcCoord.x;
    lcCoords.y = status.lcCoord.y;
    std::cout << "lc 위치: " << lcCoords.x << ", " << lcCoords.y << std::endl;
}

void Mfr::parsingSimData(const std::vector<char>& payload)
{
    if (payload.size() != sizeof(SimData)) 
    {
        std::cerr << "[Mfr::handleSimDataPayload] SimData 크기 오류. 받은 크기: " << payload.size() << std::endl;
        return;
    }
    
    SimData data;
    std::memcpy(&data, payload.data(), sizeof(SimData));    
    // std::cout << "[Mfr::handleSimDataPayload] SimData 수신 -> ID: " << data.mockId
    //           << ", Latitude: " << data.mockCoords.latitude
    //           << ", Longitude: " << data.mockCoords.longitude
    //           << ", Altitude: " << data.mockCoords.altitude
    //           << ", Angle: " << data.angle
    //           << ", Speed: " << data.speed << std::endl;


    localSimData localSimData;
    localSimData.mockId = data.mockId;
    localSimData.mockCoords = decode(data.mockCoords);
    localSimData.angle = data.angle;
    localSimData.speed = data.speed;

    // std::cout << "latitude: " << localSimData.mockCoords.latitude << ", longitude: " << localSimData.mockCoords.longitude << ", altitude: " << localSimData.mockCoords.altitude << std::endl;

    if (localSimData.mockId >= 101001 && localSimData.mockId <= 101999)     // 표적 정보
    {
        // std::cout << "[Mfr::handleSimDataPayload] -> 모의 표적 등록 완료" << std::endl;
        addMockTarget(localSimData);        
    }
    else if (localSimData.mockId >= 102001 && localSimData.mockId <= 102999)        // 미사일 정보
    {
        // std::cout << "[Mfr::handleSimDataPayload] -> 모의 미사일 등록 완료" << std::endl;
        addMockMissile(localSimData);
    }
    else 
    {
        std::cerr << "[Mfr::handleSimDataPayload] 알 수 없는 ID 범위: " << localSimData.mockId << std::endl;
    }
}

uint64_t Mfr::toEpochMillis(std::chrono::system_clock::time_point tp)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
}

void Mfr::startDetectionAlgoThread()
{
    detectionThreadRunning = true;

    detectionThread = std::thread([this]() {
        while (detectionThreadRunning) 
        {
            mfrDetectionAlgo();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void Mfr::stopDetectionAlgoThread()
{
    detectionThreadRunning = false;
    if (detectionThread.joinable())
    {
        detectionThread.join();
    }
}

double Mfr::calcDistance(Pos3D mfrCoord, Pos3D mockCoord)
{
    // 위도, 경도 (단위: degree)
    double mfrLatitudeDeg = mfrCoord.latitude;
    double mfrLongitudeDeg = mfrCoord.longitude;
    double mockLatitudeDeg = mockCoord.latitude;
    double mockLongitudeDeg = mockCoord.longitude;

    // 위도, 경도 (단위: radian)
    double mfrLatitudeRad = deg2rad(mfrLatitudeDeg);
    double mockLatitudeRad = deg2rad(mockLatitudeDeg);

    // 위도, 경도 변화율 (단위: radian)
    double deltaLatitude = deg2rad(mockLatitudeDeg - mfrLatitudeDeg);
    double deltaLongitude = deg2rad(mockLongitudeDeg - mfrLongitudeDeg);

    // Haversine Formula 적용
    double sinDelataLatitude = std::sin(deltaLatitude / 2.0);
    double sinDelataLongitude = std::sin(deltaLongitude / 2.0);
    
    double a = sinDelataLatitude * sinDelataLatitude + std::cos(mfrLatitudeRad) * std::cos(mockLatitudeRad) * sinDelataLongitude * sinDelataLongitude;
    double c = 2.0 * std::asin(std::sqrt(a));

    double horizontal = EARTH_RADIUS_M * c;
    double vertical = mockCoord.altitude - mfrCoord.altitude;
    double dist = std::sqrt(horizontal * horizontal + vertical * vertical);    

    return dist;
}

double Mfr::calcAngleToTarget(long long x1, long long y1, long long x2, long long y2)
{
    long long dx = x2 - x1;
    long long dy = y2 - y1;
    double angle = std::atan2(dy, dx) * 180.0 / M_PI;
    return angle < 0 ? angle + 360.0 : angle;
}

bool Mfr::isInFoV(double targetAngle)
{
    double diff = std::fabs(targetAngle - goalMotorAngle);
    
    if (diff > 180.0) 
    {
        diff = 360.0 - diff;
    }

    return diff <= limitedFoV;
}

void Mfr::mfrDetectionAlgo()
{
    std::unordered_map<unsigned int, localSimData> localTargets;
    std::unordered_map<unsigned int, localSimData> localMissiles;

    {
        std::shared_lock<std::shared_mutex> lock(mockTargetMutex);
        localTargets = mockTargets;
    }

    {
        std::shared_lock<std::shared_mutex> lock(mockMissileMutex);
        localMissiles = mockMissile;
    }

    std::unordered_map<unsigned int, localSimData> localDetectedTargets;
    std::unordered_map<unsigned int, localSimData> localDetectedMissile;

    std::vector<MfrToLcTargetInfo> detectedTargetList;
    std::vector<MfrToLcMissileInfo> detectedMissileList;

    auto now = std::chrono::system_clock::now();
    uint64_t nowMs = toEpochMillis(now);
    // std::cout << "SimData SIZE: " << sizeof(MfrToLcTargetInfo) << std::endl;
    // std::cout << "[Mfr::mfrDetectionAlgo] 현재 시간: " << nowMs << " ms, " << "targetSize: " << localTargets.size();
    if (mfrMode == ROTATION_MODE)
    {
        // std::cout << ", mfrMode: ROTATION_MODE" << std::endl;
        std::vector<std::pair<unsigned int, long long>> targetDistances;

        for (const auto& [id, target] : localTargets)
        {
            double distance = calcDistance(mfrCoords, target.mockCoords);
            
            if (distance <= limitDetectionRange)
            {
                localDetectedTargets[id] = target;
                targetDistances.emplace_back(id, distance);
                std::cout << "[Mfr::mfrDetectionAlgo] Target ID: " << id 
                          << ", Distance: " << distance 
                          << ", Coords: (" << target.mockCoords.latitude << ", " << target.mockCoords.longitude << ", " << target.mockCoords.altitude << ")" 
                          << std::endl;
            }
            // else if(distance > maxLimitDetectionRange)
            // {
            //     {
            //         std::unique_lock lock(mockTargetMutex);
            //         mockTargets.erase(id);
            //     }                
            //     localTargets.erase(id);
            // }
            // std::cout << "target here: " << "distance: " << distance << std::endl;
        }
        
        std::sort(targetDistances.begin(), targetDistances.end(), [](const auto& a, const auto& b) 
        { 
            return a.second < b.second;
        });

        unsigned char priority = 1;
        for (const auto& [id, distance] : targetDistances)
        {
            const auto& target = localDetectedTargets[id];

            if(priority == 1)
            {
                goalTargetId = id;
            }

            MfrToLcTargetInfo status{};
            status.id = target.mockId;
            status.targetCoords = encode(target.mockCoords);
            status.targetSpeed = target.speed;
            status.targetAngle = target.angle;
            status.firstDetectionTime = nowMs;
            status.prioirty = priority++;
            status.isHit = false;

            detectedTargetList.push_back(status);
        }

        for (const auto& [id, missile] : localMissiles)
        {
            long long distance = calcDistance(mfrCoords, missile.mockCoords);
            if (distance <= limitDetectionRange)
            {
                localDetectedMissile[id] = missile;

                std::cout << "[Mfr::mfrDetectionAlgo] Missile ID: " << id 
                          << ", Distance: " << distance 
                          << ", Coords: (" << missile.mockCoords.latitude << ", " << missile.mockCoords.longitude << ", " << missile.mockCoords.altitude << ")" 
                          << std::endl;

                MfrToLcMissileInfo status{};
                status.id = missile.mockId;
                status.missileCoords = encode(missile.mockCoords);
                status.missileSpeed = missile.speed;
                status.missileAngle = missile.angle;
                status.isHit = false;

                detectedMissileList.push_back(status);
            }
            std::cout << "missile here: " << "distance: " << distance << std::endl;
        }
    }

    else if (mfrMode == ANGLE_MODE)
    {
        std::cout << "mfrMode: ANGLE_MODE" << std::endl;
        detectedTargetList.clear();
        detectedMissileList.clear();
        if (localTargets.find(goalTargetId) != localTargets.end())
        {
            const auto& goalTarget = localTargets[goalTargetId];
            //double goalAngle = calcAngleToTarget(mfrCoords.latitude, mfrCoords.longitude, goalTarget.mockCoords.latitude, goalTarget.mockCoords.longitude);
            //goalMotorAngle = goalAngle;

            for (const auto& [id, target] : localTargets)
            {
                //double angle = calcAngleToTarget(mfrCoords.latitude, mfrCoords.longitude, target.mockCoords.latitude, target.mockCoords.longitude);
                long long distance = calcDistance(mfrCoords, target.mockCoords);

                if (distance <= limitDetectionRange)    // && isInFoV(angle)
                {
                    localDetectedTargets[id] = target;

                    std::cout << "[Mfr::mfrDetectionAlgo] Target ID: " << id 
                              << ", Distance: " << distance 
                              << ", Coords: (" << target.mockCoords.latitude << ", " << target.mockCoords.longitude << ", " << target.mockCoords.altitude << ")"
                              << std::endl;

                    MfrToLcTargetInfo status{};
                    status.id = target.mockId;
                    status.targetCoords = encode(target.mockCoords);
                    status.targetSpeed = target.speed;
                    status.targetAngle = target.angle;
                    status.firstDetectionTime = nowMs;
                    status.prioirty = 1;
                    status.isHit = false;

                    detectedTargetList.push_back(status);
                }
            }

            for (const auto& [id, missile] : localMissiles)
            {
                // double angle = calcAngleToTarget(mfrCoords.x, mfrCoords.y, missile.mockCoords.x, missile.mockCoords.y);
                long long distance = calcDistance(mfrCoords, missile.mockCoords);

                if (distance <= limitDetectionRange)    // && isInFoV(angle)
                {
                    localDetectedMissile[id] = missile;

                    std::cout << "[Mfr::mfrDetectionAlgo] Missile ID: " << id 
                              << ", Distance: " << distance 
                              << ", Coords: (" << missile.mockCoords.latitude << ", " << missile.mockCoords.longitude << ", " << missile.mockCoords.altitude << ")" 
                              << std::endl;

                    MfrToLcMissileInfo status{};
                    status.id = missile.mockId;
                    status.missileCoords = encode(missile.mockCoords);
                    status.missileSpeed = missile.speed;
                    status.missileAngle = missile.angle;
                    status.isHit = false;

                    detectedMissileList.push_back(status);
                }
            }
        }
    }
    
    if (!detectedTargetList.empty() || !detectedMissileList.empty())
    {
        std::vector<char> packet = serializeDetectionPacket(detectedTargetList, detectedMissileList);        
        // lcCommManager->send(packet);
    }

    {
        std::unique_lock<std::shared_mutex> lock(detectedTargetMutex);
        detectedTargets = std::move(localDetectedTargets);
        std::cout << "[Mfr::mfrDetectionAlgo] 탐지된 표적 수: " << detectedTargets.size() << std::endl;
    }    

    {
        std::unique_lock<std::shared_mutex> lock(detectedMissileMutex);
        detectedMissile = std::move(localDetectedMissile);
    }
}

void Mfr::addMockTarget(const localSimData& target) 
{
    std::unique_lock<std::shared_mutex> lock(mockTargetMutex);
    mockTargets[target.mockId] = target;
}

//
//  detected 된 데이터에서만 조회
//
localSimData* Mfr::getMockTargetById(unsigned int id) 
{
    std::shared_lock<std::shared_mutex> lock(detectedTargetMutex);
    auto it = detectedTargets.find(id);
    return (it != detectedTargets.end()) ? &it->second : nullptr;
}

void Mfr::removeMockTargetById(unsigned int id) 
{
    {
        std::unique_lock<std::shared_mutex> lock(mockTargetMutex);
        mockTargets.erase(id);
    }

    {
        std::unique_lock<std::shared_mutex> lock(detectedTargetMutex);
        detectedTargets.erase(id);
    }
}

void Mfr::clearMockTargets() 
{
    {
        std::unique_lock<std::shared_mutex> lock(mockTargetMutex);
        mockTargets.clear();
    }

    {
        std::unique_lock<std::shared_mutex> lock(detectedTargetMutex);
        detectedTargets.clear();
    }
}

void Mfr::addMockMissile(const localSimData& missile) 
{
    {
        std::unique_lock<std::shared_mutex> lock(mockMissileMutex);
        mockMissile[missile.mockId] = missile;
    }

    {
        std::unique_lock<std::shared_mutex> lock(detectedMissileMutex);
        detectedMissile[missile.mockId] = missile;
    }
}

//
//  detected 된 데이터에서만 조회
//
localSimData* Mfr::getMockMissileById(unsigned int id)
{
    std::shared_lock<std::shared_mutex> lock(detectedMissileMutex);
    auto it = detectedMissile.find(id);
    return (it != detectedMissile.end()) ? &it->second : nullptr;
}

void Mfr::removeMockMissileById(unsigned int id) 
{
    {
        std::unique_lock<std::shared_mutex> lock(mockMissileMutex);
        mockMissile.erase(id);
    }

    {
        std::unique_lock<std::shared_mutex> lock(detectedMissileMutex);
        detectedMissile.erase(id);
    }
}

void Mfr::clearMockMissiles() 
{
    {
        std::unique_lock<std::shared_mutex> lock(mockMissileMutex);
        mockMissile.clear();
    }

    {
        std::unique_lock<std::shared_mutex> lock(detectedMissileMutex);
        detectedMissile.clear();
    }
}