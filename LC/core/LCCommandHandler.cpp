#include "LCCommandHandler.h"
#include "LCManager.h"
#include "Serializer.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <climits>

namespace LCCommandHandler
{
    using namespace Common;

    // ECC 명령 처리
    void handleECCCommand(const CommonMessage &msg, LCManager &manager)
    {
        switch (msg.commandType)
        {
        case CommandType::STATUS_REQUEST_ECC_TO_LC: {
            static int eccRequestCounter = 0;
            eccRequestCounter++;

            // if (eccRequestCounter % 10 == 0) {
            //     std::cout << "[ECC] STATUS_REQUEST 수신 → 상태 전송\n";
            // }
            manager.sendStatus();
            break;
        }
        //0x02
        case CommandType::SET_RADAR_MODE_ECC_TO_LC:
        {
            const auto& payload = std::get<RadarModeCommand>(msg.payload);

            std::cout << "[ECC] 레이더 모드 변경 수신 → "
                    << "radarId=" << payload.radarId
                    << ", mode=" << static_cast<int>(payload.radarMode)
                    << ", priority_select=" << static_cast<int>(payload.priority_select)
                    << ", targetId=" << payload.targetId << "\n";

            // 직렬화 및 전송
            auto packet = Serializer::serializeRadarModeChange(payload);
            if (manager.hasMFRSender()) {
                manager.sendToMFR(packet);
                std::cout << "레이더에게 모드변경 요청, 전송 Byte : " << packet.size() << "\n";  
            } else {
                std::cerr << "[LCCommandHandler] MFR 송신자 없음. 전송 실패\n";
            }
            break;
        }
        //0x03
        case CommandType::SET_LAUNCHER_MODE_ECC_TO_LC:
        {
            const auto &payload = std::get<LauncherCommand>(msg.payload);
            std::cout << "[ECC] 발사대 모드 변경 수신 → lsId=" << payload.lsId
                      << ", mode=" << static_cast<int>(payload.lsMode) << "\n";

            LauncherModeCommand cmd;
            cmd.launcherId = payload.lsId;
            cmd.newMode = static_cast<OperationMode>(payload.lsMode);

            auto packet = Serializer::serializeModeChangeCommand(cmd);
            if (manager.hasLSSender())
            {
                manager.sendToLS(packet);
            }
            break;
        }
        // 0x04
        case CommandType::FIRE_COMMAND_ECC_TO_LC:
        {
            const auto& payload = std::get<FireCommand>(msg.payload);
            std::cout << "[ECC] 발사 명령 수신 → lsId=" << payload.lsId
                    << ", targetId=" << payload.targetId << "\n";

            SystemStatus snapshot = manager.getStatusCopy();
            const auto& ls = snapshot.ls;
            const auto& targets = snapshot.targets;

            TargetStatus selectedTarget{};
            bool found = false;

            // 타겟 선택 (0이면 가장 가까운 타겟)
            if (payload.targetId == 0) {
                long long minDistSq = LLONG_MAX;
                for (const auto& t : targets) {
                    long long dx = t.posX - ls.position.x;
                    long long dy = t.posY - ls.position.y;
                    long long distSq = dx * dx + dy * dy;
                    if (distSq < minDistSq) {
                        minDistSq = distSq;
                        selectedTarget = t;
                        found = true;
                    }
                }
            } else {
                for (const auto& t : targets) {
                    if (t.id == payload.targetId) {
                        selectedTarget = t;
                        found = true;
                        break;
                    }
                }
            }

            if (!found) {
                std::cerr << "[LC] 대상 타겟 없음 → targetId=" << payload.targetId << "\n";
                break;
            }

            LaunchCommand cmd;
            cmd.launcherId = ls.launchSystemId;

            // 상대 위치 및 속도
            double dx = static_cast<double>(selectedTarget.posX - ls.position.x);
            double dy = static_cast<double>(selectedTarget.posY - ls.position.y);

            double targetSpeed = static_cast<double>(selectedTarget.speed);
            double targetAngleRad = selectedTarget.angle * M_PI / 180.0;
            double vx = targetSpeed * std::cos(targetAngleRad);
            double vy = targetSpeed * std::sin(targetAngleRad);

            double missileSpeed = 40000.0; // 단위: 좌표계 변화량/초 (실제 단위는 신경쓰지 않음)

            // 요격 각도 계산 (벡터 내적 공식)
            double a = vx * vx + vy * vy - missileSpeed * missileSpeed;
            double b = 2 * (dx * vx + dy * vy);
            double c = dx * dx + dy * dy;

            double discriminant = b * b - 4 * a * c;
            double t_impact = -1.0;

            if (a == 0) {
                // 타겟이 정지해 있거나, 미사일 속도와 타겟 속도가 같음
                if (b != 0)
                    t_impact = -c / b;
            } else if (discriminant >= 0) {
                double t1 = (-b + std::sqrt(discriminant)) / (2 * a);
                double t2 = (-b - std::sqrt(discriminant)) / (2 * a);
                // 양수 중 최소값 선택
                if (t1 > 0 && t2 > 0)
                    t_impact = std::min(t1, t2);
                else if (t1 > 0)
                    t_impact = t1;
                else if (t2 > 0)
                    t_impact = t2;
            }

            double launchAngleXY = 0.0;
            if (t_impact > 0) {
                double interceptX = dx + vx * t_impact;
                double interceptY = dy + vy * t_impact;
                launchAngleXY = std::atan2(interceptY, interceptX) * 180.0 / M_PI;
            } else {
                // 요격 불가: 가장 가까운 방향으로 쏨
                launchAngleXY = std::atan2(dy, dx) * 180.0 / M_PI;
                t_impact = 0.0;
                std::cerr << "[LC] 요격 불가: 타겟 속도가 너무 빠르거나, 미사일 속도가 너무 느림\n";
            }

            cmd.launchAngleXY = launchAngleXY;
            cmd.launchAngleXZ = 0.0; // z축 무시

            std::cout << "[LC] 최적 조준 결과\n"
                    << "  조준 각도 (XY): " << cmd.launchAngleXY << "도\n"
                    << "  추정 요격 시간: " << t_impact << "초\n";

            auto packet = Serializer::serializeLaunchCommand(cmd);

            if (manager.hasLSSender()) {
                manager.sendToLS(packet);
            } else {
                std::cerr << "[LC] LS 송신자 없음. 전송 실패\n";
            }

            std::cout << std::dec;
            std::cout << "------------------------------------------------------" << std::endl;
            std::cout << "발사명령 정보" << "\n"
                    << "  lsId: " << cmd.launcherId
                    << ", launchAngleXY: " << cmd.launchAngleXY
                    << ", launchAngleXZ: " << cmd.launchAngleXZ << " (더미값)\n";
            std::cout << "------------------------------------------------------" << std::endl;

            break;
        }
        


        //0x05
        case CommandType::MOVE_COMMAND_ECC_TO_LC:
        {
            const auto &payload = std::get<MoveCommand>(msg.payload);
            std::cout << "[ECC] 이동 명령 수신 → lsId=" << payload.lsId
                      << ", x=" << payload.posX << ", y=" << payload.posY << "\n";

            MoveCommandLS cmd;
            cmd.launcherId = payload.lsId;
            cmd.newX = payload.posX;
            cmd.newY = payload.posY;

            auto packet = Serializer::serializeMoveCommandLS(cmd);
            if (manager.hasLSSender())
            {
                manager.sendToLS(packet);
            }
            break;
        }

        default:
            std::cerr << "[ECC] 알 수 없는 명령\n";
            break;
        }
    }

    // MFR 명령 처리
    void handleMFRCommand(const CommonMessage &msg, LCManager &manager)
    {
        switch (msg.commandType)
        {
        case CommandType::STATUS_RESPONSE_MFR_TO_LC:
            manager.onRadarStatusReceived(std::get<RadarStatus>(msg.payload));
            break;
        case CommandType::DETECTION_MFR_TO_LC:  {
            manager.onRadarDetectionReceived(std::get<RadarDetection>(msg.payload));
            break;
        }
        case CommandType::POSITION_REQUEST_MFR_TO_LC:
        {
            manager.onLCPositionRequest();
            break;
        }
        default:
            std::cerr << "[MFR] 알 수 없는 명령\n";
            break;
        }
    }

    // LS 명령 처리
    void handleLSCommand(const CommonMessage &msg, LCManager &manager)
    {
        switch (msg.commandType)
        {
        case CommandType::LS_STATUS_UPDATE_LS_TO_LC:
        {
            // std::cout <<"data received from LS\n" ;
            const auto &status = std::get<Common::LSReport>(msg.payload);
            manager.onLSStatusReceived(status); // ✅ MFR처럼 위임 방식으로 통일
            break;
        }
        default:
            std::cerr << "[LS] 처리되지 않은 명령 수신: " << static_cast<int>(msg.commandType) << "\n";
            break;
        }
    }
    // LC 위치 요청 처리
        // void handleLCPositionRequest(LCManager &manager)
        // {
        //     manager.onLCPositionRequest();  // 내부에서 직렬화, sendToMFR()까지 처리
        // }
        // 통합 명령 처리 진입점
        void handleCommand(SenderType sender, const CommonMessage &msg, LCManager &manager)
        {
            switch (sender)
            {
            case SenderType::ECC:
                handleECCCommand(msg, manager);
                break;
            case SenderType::MFR:
                handleMFRCommand(msg, manager);
                break;
            case SenderType::LS:
                handleLSCommand(msg, manager);
                break;
            default:
                std::cerr << "[LCCommandHandler] 알 수 없는 송신자\n";
                break;
            }
        }


    // void handleLSCommand(const CommonMessage& msg, LCManager& manager) {
    //     switch (msg.commandType) {
    //     case CommandType::SET_LAUNCHER_MODE: {  // 0x03
    //         const auto& payload = std::get<LauncherCommand>(msg.payload);
    //         std::cout << "[ECC] 발사대 모드 변경 수신 → lsId=" << payload.lsId
    //                 << ", mode=" << static_cast<int>(payload.lsMode) << "\n";

    //         LSStatus status = manager.getStatusCopy().ls;
    //         status.lsId = payload.lsId;
    //         status.mode = payload.lsMode;
    //         // manager.updateStatus(status);
    //         break;
    //     }
    //     case CommandType::FIRE_COMMAND: {
    //         const auto& payload = std::get<FireCommand>(msg.payload);
    //         std::cout << "[LS] 발사 명령: lsId=" << payload.lsId
    //                   << ", targetId=" << payload.targetId << "\n";
    //         // 실제 발사 로직 필요시 추가
    //         break;
    //     }
    //     case CommandType::MOVE_COMMAND: {
    //         const auto& payload = std::get<MoveCommand>(msg.payload);
    //         std::cout << "[LS] 이동 명령: lsId=" << payload.lsId
    //                   << ", x=" << payload.posX << ", y=" << payload.posY << "\n";
    //         // 실제 이동 로직 필요시 추가
    //         break;
    //     }
    //     case CommandType::STATUS_REQUEST:
    //         std::cout << "[LS] 상태 요청 수신 → 응답 전송\n";
    //         manager.sendStatus();
    //         break;
    //     default:
    //         std::cerr << "[LS] 알 수 없는 명령 수신\n";
    //         break;
    //     }
    // }

  

} // namespace LCCommandHandler