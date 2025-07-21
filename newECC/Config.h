#ifndef ECC_CONFIG_H
#define ECC_CONFIG_H

#include <string>

struct ConfigCommon {
  std::string LCSendIP;
  int LCSendPort = 0;  // Launch Controller IP and Port

  std::string MAPSendIP;
  int MAPSendPort = 0;  // Map Server IP and Port
};

// ���� ���� �� ���ڿ� ó�� ��ƿ �Լ� ����
std::string trim(const std::string &s);
std::string toLower(const std::string &s);

// ini ������ �о� MfrConfig ����ü�� ä��
bool loadConfig(const std::string &filepath, ConfigCommon &config);

#endif  // ECC_CONFIG_H
