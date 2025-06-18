#include "pch.h"
#include "ECC_TCP.h"
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

    // Map Connect
    map_tcp = std::make_unique<MAP_TCP>();
    if (map_tcp->connect("192.168.0.62", 9001))
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

    if (len > 0 && self->m_receiver) {
      self->m_receiver->receive(len, buf);
      // 받은 buf를 그대로 map_tcp로 전송
      if (self->map_tcp) {
        self->map_tcp->send(buf, len);
      }
    } else if (len == 0 || len == SOCKET_ERROR) {
      break;  // 연결 종료 또는 에러 발생
    }
  }

  return 0;
}

void ECC_TCP::stop() {
    m_bRunning = false;

    if (m_sock != INVALID_SOCKET) {
        shutdown(m_sock, SD_BOTH);  // 수신 차단
        closesocket(m_sock);
        m_sock = INVALID_SOCKET;
    }

    if (m_hThread) {
        WaitForSingleObject(m_hThread, 1000); // 최대 1초 대기
        CloseHandle(m_hThread);
        m_hThread = nullptr;
    }

    WSACleanup();
}
