#pragma once

#include "CEGUIBase.h"

namespace CEGUI
{

class EventArgs
{
public:
	EventArgs(void) : handled(0) {}
	//可被继承的类都应该是virtual类型
	virtual ~EventArgs(void) {}
	//事件是否被处理，以及被处理的次数
    uint handled;
};

}