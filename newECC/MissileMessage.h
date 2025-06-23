//#pragma once
//#include "CommonTypes.h"
//
//namespace Missile
//{
//    enum class CommandType : uint8_t {
//        INFO = 0,                       // ���� ��û
//        MISSILE_FIRE = 1,               // �̻��� �߻�
//    };
//
//    struct InfoPacket {
//        unsigned int id;                // �̻��� �ĺ���
//        Pos2D position;                 // ��ġ
//        long long z;                    // ��
//        long long speed;                // �ӵ�
//        double angle;                   // ����
//        int shoot_down_time;            // ���� �ð�
//    };
//
//    struct Message {
//        CommandType commandType;        // ��� ����
//        InfoPacket data;                // ��� �Ǵ� ���信 ���� ����
//
//    };
//}