//#pragma once
//#include "CommonTypes.h"
//
//namespace MFR
//{
//    enum class CommandType : uint8_t {
//        INFO = 0,                       // ���� ��û
//        MODE_CHANGE = 1,                // ��� ���� ��û
//        ANGLE_CHANGE = 2,               // ���� ���� ��û
//    };
//
//    enum class Mode : uint8_t {
//        STOP_MODE = 0,
//        ROTATE_MODE = 1
//    };
//
//    struct InfoPacket {
//        unsigned int id;                // �߻�� �ĺ���
//        Pos2D position;                 // ��ġ
//        double angle;                   // Ž�� ��
//        Mode mode;                      // ���� ���
//    };
//
//    struct Message {
//        CommandType commandType;        // ��� ����
//        InfoPacket data;                // ��� �Ǵ� ���信 ���� ����
//
//    };
//
//}