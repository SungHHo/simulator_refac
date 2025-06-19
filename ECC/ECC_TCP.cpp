#include "pch.h"
#include "ECC_TCP.h"
#include "Config.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <iostream>

#include "MAP_TCP.h"
ECC_TCP::ECC_TCP() {}

ECC_TCP::~ECC_TCP() {
    stop();
}

bool ECC_TCP::connect(const char* ip, int port)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return false;
    }

    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);

    if (::connect(m_sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(m_sock);
        WSACleanup();
        return false;
    }

    ConfigCommon config;
    if (!loadConfig("ECCconfig.ini", config)) {
      AfxMessageBox(_T("���� ������ �о���� �� �����߽��ϴ�."));
      return FALSE;  // �ʱ�ȭ ����
    }

    // Map Connect
    map_tcp = std::make_unique<MAP_TCP>();
    if (map_tcp->connect(config.MAPSendIP, config.MAPSendPort))
    {
      std::cout << "success Map TCP Connect" << std::endl;
    }

    return true;
}

void ECC_TCP::registerReceiver(IReceiver* receiver) {
    m_receiver = receiver;
}

void ECC_TCP::send(const char* data, int len) {
    if (m_sock != INVALID_SOCKET) {
        ::send(m_sock, data, len, 0);
    }
}

void ECC_TCP::startReceiving() {
    if (!m_bRunning) {
        m_bRunning = true;
        m_hThread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, recvThread, this, 0, nullptr));
    }
}

unsigned __stdcall ECC_TCP::recvThread(void* arg) {
    ECC_TCP* self = static_cast<ECC_TCP*>(arg);
    char buf[3072];

  while (self->m_bRunning) {
    int len = recv(self->m_sock, buf, sizeof(buf), 0);

        // �� 4����Ʈ�� 0x51 0x01 0x01 0x01���� Ȯ��
    if (!(static_cast<unsigned char>(buf[0]) == 0x51 &&
          static_cast<unsigned char>(buf[1]) == 0x01 &&
          static_cast<unsigned char>(buf[2]) == 0x01 &&
          static_cast<unsigned char>(buf[3]) == 0x01)) {
      continue;  // ���� ����ġ �� ����
    }

    if (len > 0 && self->m_receiver) {
      self->m_receiver->receive(len, buf);
      // ���� buf�� �״�� map_tcp�� ����
      if (self->map_tcp) {
        self->map_tcp->send(buf, len);
        std::cout << "first 5 bytes (hex): ";
        for (int i = 0; i < 5 && i < len; ++i) {
          printf("%02X ", static_cast<unsigned char>(buf[i]));
        }
        std::cout << "buf size : " << len << std::endl;
      }
    } else if (len == 0 || len == SOCKET_ERROR) {
      break;  // ���� ���� �Ǵ� ���� �߻�
    }
  }

  return 0;
}

void ECC_TCP::stop() {
    m_bRunning = false;

    if (m_sock != INVALID_SOCKET) {
        shutdown(m_sock, SD_BOTH);  // ���� ����
        closesocket(m_sock);
        m_sock = INVALID_SOCKET;
    }

    if (m_hThread) {
        WaitForSingleObject(m_hThread, 1000); // �ִ� 1�� ���
        CloseHandle(m_hThread);
        m_hThread = nullptr;
    }

    WSACleanup();
}
