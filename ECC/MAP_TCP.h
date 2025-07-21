#pragma once
#include <string>

class MAP_TCP {
 public:
  MAP_TCP();
  ~MAP_TCP();

  // IP�� ��Ʈ�� �޾� ������ ���� �õ�, ���� �� true ��ȯ
  bool connect(const std::string& ip, uint16_t port);

  // ����� ���� ��ũ���� ��ȯ (������� �ʾ����� -1)
  int getSocket() const;

  // ���� ����
  void disconnect();

  // ������ ���� �Լ� �߰�
  // ���������� ������ ����Ʈ �� ��ȯ, ���� �� -1 ��ȯ
  int send(const char* data, int len);

 private:
  int m_socket;
};