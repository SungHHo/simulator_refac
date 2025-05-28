#include "LCCommandHandler.h"
#include "LCManager.h"
#include "Serializer.h"
#include <iostream>
#include <vector>
#include <cstring>

namespace LCCommandHandler
{
    using namespace Common;

    // ECC 명령 처리
    void handleECCCommand(const CommonMessage &msg, LCManager &manager)
    {
        switch (msg.commandType)
        {

        case CommandType::STATUS_REQUEST_ECC_TO_LC: 
            std::cout << "[ECC] STATUS_REQUEST 수신 → 상태 전송\n";
            manager.sendStatus();
            break;
        //0x02
        case CommandType::SET_RADAR_MODE_ECC_TO_LC:
        {
            const auto &payload = std::get<RadarModeCommand>(msg.payload);

            std::cout << "[ECC] 레이더 모드 변경 수신 → radarId=" << payload.radarId
                      << ", mode=" << static_cast<int>(payload.radarMode);

            if (payload.radarMode == 0)
            { // STOP 모드일 경우만 추가 정보 출력
                std::cout << ", flag=" << static_cast<int>(payload.flag);
                if (payload.flag == 0x02)
                {
                    std::cout << ", targetId=" << payload.targetId;
                }
                else if (payload.flag != 0x01)
                {
                    std::cerr << " 잘못된 flag 값: " << static_cast<int>(payload.flag) << "\n";
                }
            }

            std::cout << "\n";

            // 직렬화 및 전송
            auto packet = Serializer::serializeRadarModeChange(payload);
            if (manager.hasMFRSender())
            {
                manager.sendToMFR(packet);
            }
            else
            {
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

            // 직렬화
            auto packet = Serializer::serializeFireCommandToLS(payload);
            
            // LS로 전송
            if (manager.hasLSSender()) {
                manager.sendToLS(packet);
            } else {
                std::cerr << "[LCCommandHandler] LS 송신자 없음. 전송 실패\n";
            }
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
            std::cout <<"data received from LS\n" ;
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