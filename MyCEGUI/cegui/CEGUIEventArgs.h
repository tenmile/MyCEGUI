#pragma once

#include "CEGUIBase.h"

namespace CEGUI
{

class EventArgs
{
public:
	EventArgs(void) : handled(0) {}
	//�ɱ��̳е��඼Ӧ����virtual����
	virtual ~EventArgs(void) {}
	//�¼��Ƿ񱻴����Լ�������Ĵ���
    uint handled;
};

}