#pragma once
#include "ReceiveMessageTypes.h"

// STATUS_REQUEST ���� ��Ŷ �ļ�
ParsedStatusResponse ParseStatusResponse(const char* buffer, size_t len);
