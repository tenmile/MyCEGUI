#pragma once

#include "CEGUIBase.h"

namespace CEGUI
{

class XMLHandler
{
public:
    XMLHandler(void);
	//virtual
    virtual ~XMLHandler(void);
    //回调函数
    virtual void elementStart(const String& element, const XMLAttributes& attributes);
    //回调函数
    virtual void elementEnd(const String& element);
	//回调函数
	virtual void text(const String& text);
};

}