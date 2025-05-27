//#pragma once
//#include "CommonTypes.h"
//
//namespace LS {
//    enum class CommandType : uint8_t {
//        INFO = 0,                       // ���� ��û
//        MODE_CHANGE = 1,                // ��� ���� ��û
//        MISSILE_LAUNCH = 2,               // �̻��� �߻� ��û
//        MOVE_POS = 3,                   // ��ġ ���� ��û
//    };
//
//    enum class Mode : uint8_t {
//        STOP_MODE = 0,
//        MOVE_MODE = 1,
//        WAR_MODE = 2
//    };
//
//    struct InfoPacket {
//        unsigned int id;                // �߻�� �ĺ���
//        Pos2D position;                 // ��ġ
//        double angle;                   // �߻� ��
//        Mode mode;                      // ���� ���
//        int speed;			// �̻��� �߻� �ӵ�
//
//    };
//
//    struct Message {
//        CommandType commandType;        // ��� ����
//        InfoPacket data;                // ��� �Ǵ� ���信 ���� ����
//    };
//}