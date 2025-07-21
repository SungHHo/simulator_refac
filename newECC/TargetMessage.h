//#pragma once
//#include "CommonTypes.h"
//
//namespace Target {
//    enum class CommandType : uint8_t {
//        INFO = 0,                       // ���� ��û
//    };
//
//    struct InfoPacket {
//        unsigned int id;                // �̻��� �ĺ���
//        Pos2D position;                 // ��ġ
//        long long z;                    // ��
//        long long speed;                // �ӵ�
//        double angle;                   // ����
//        int shoot_down_time;            // ���� �켱 ����
//    };
//
//    struct Message {
//        CommandType commandType;        // ��� ����
//        std::vector<InfoPacket> data;   // ��� �Ǵ� ���信 ���� ����
//
//    };
//}