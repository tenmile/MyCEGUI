#include "CEGUIGlobalEventSet.h"
#include "CEGUILogger.h"


namespace CEGUI
{

template<> GlobalEventSet* Singleton<GlobalEventSet>::ms_Singleton	= 0;

GlobalEventSet::GlobalEventSet()
{
    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
	Logger::getSingleton().logEvent(
        "CEGUI::GlobalEventSet singleton created. " + String(addr_buff));
}

GlobalEventSet::~GlobalEventSet()
{
    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
	Logger::getSingleton().logEvent(
        "CEGUI::GlobalEventSet singleton destroyed. " + String(addr_buff));
}

GlobalEventSet&	GlobalEventSet::getSingleton(void)
{
	return Singleton<GlobalEventSet>::getSingleton();
}

GlobalEventSet*	GlobalEventSet::getSingletonPtr(void)
{
	return Singleton<GlobalEventSet>::getSingletonPtr();
}

/*************************************************************************
	Overridden fireEvent which always succeeds.
*************************************************************************/
void GlobalEventSet::fireEvent(const String& name, EventArgs& args, const String& eventNamespace)   //全局事件名和普通事件名只是前缀不同
{
    fireEvent_impl(eventNamespace + "/" + name, args);
}

}