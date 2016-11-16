#pragma once

#include "CEGUIEventSet.h"
#include "CEGUISingleton.h"

namespace CEGUI
{
/*!
\brief
    The GlobalEventSet singleton allows you to subscribe to an event for all
    instances of a class.  The GlobalEventSet effectively supports "late binding"
    to events; which means you can subscribe to some event that does not actually
    exist (yet).
*/
class GlobalEventSet : public EventSet, public Singleton<GlobalEventSet>
{
public:
	GlobalEventSet();
	~GlobalEventSet();
	static	GlobalEventSet&	getSingleton(void);
	static	GlobalEventSet*	getSingletonPtr(void);
	virtual void	fireEvent(const String& name, EventArgs& args, const String& eventNamespace = "");
};

}