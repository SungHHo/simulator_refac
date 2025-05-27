//#include "pch.h"
//#include "TcpClient.h"
//
//CTcpClient::CTcpClient()
//{
//    m_socket = INVALID_SOCKET;
//    m_pThread = nullptr;
//    m_bRunning = FALSE;
//    m_pReceiver = nullptr;
//}
//
//CTcpClient::~CTcpClient()
//{
//    Disconnect();
//}
//
//BOOL CTcpClient::Connect(const CString& ip, UINT port)
//{
//    static bool initialized = false;
//    if (!initialized) {
//        WSADATA wsaData;
//        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//            return FALSE;
//        initialized = true;
//    }
//
//    m_socket = socket(AF_INET, SOCK_STREAM, 0);
//    if (m_socket == INVALID_SOCKET)
//        return FALSE;
//
//    sockaddr_in serverAddr = {};
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_port = htons(port);
//
//    CStringA ansiIp = CT2A(ip);
//    inet_pton(AF_INET, ansiIp, &serverAddr.sin_addr);  // �� ����ڰ� �ѱ� ip ���
//
//    return connect(m_socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR;
//}
//
//void CTcpClient::SetReceiver(IReceiver* receiver)
//{
//    m_pReceiver = receiver;
//}
//
//void CTcpClient::SendData(const CString& data)
//{
//    CStringA ansi = CT2A(data);
//    send(m_socket, ansi.GetString(), ansi.GetLength(), 0);
//}
//
//void CTcpClient::StartReceiving()
//{
//    m_bRunning = TRUE;
//    m_pThread = AfxBeginThread(ReceiveThread, this);
//}
//
//UINT CTcpClient::ReceiveThread(LPVOID pParam)
//{
//    CTcpClient* pThis = reinterpret_cast<CTcpClient*>(pParam);
//    char buffer[2048] = {};
//
//    while (pThis->m_bRunning)
//    {
//        int len = recv(pThis->m_socket, buffer, sizeof(buffer) - 1, 0);  // -1 to null-terminate
//        if (len > 0)
//        {
//            buffer[len] = '\0'; // null terminate for logging or string usage
//
//            OutputDebugStringA("[TCP] ���ŵ�: ");
//            OutputDebugStringA(buffer);
//            OutputDebugStringA("\n");
//
//            if (pThis->m_pReceiver)
//            {
//                pThis->m_pReceiver->receive(len, buffer);
//            }
//        }
//        else if (len == 0 || WSAGetLastError() == WSAECONNRESET)
//        {
//            // ���� ����
//            OutputDebugString(L"[TCP] ������ ����Ǿ����ϴ�.\n");
//            break;
//        }
//    }
//
//    return 0;
//}
//
//void CTcpClient::Disconnect()
//{
//    m_bRunning = FALSE;
//
//    if (m_socket != INVALID_SOCKET)
//    {
//        shutdown(m_socket, SD_BOTH);
//        closesocket(m_socket);
//        m_socket = INVALID_SOCKET;
//    }
//
//    // WSACleanup�� 1���� ȣ��ǵ��� �����ϰų� ���� ����
//}
