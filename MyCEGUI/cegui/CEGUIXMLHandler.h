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
    //�ص�����
    virtual void elementStart(const String& element, const XMLAttributes& attributes);
    //�ص�����
    virtual void elementEnd(const String& element);
	//�ص�����
	virtual void text(const String& text);
};

}