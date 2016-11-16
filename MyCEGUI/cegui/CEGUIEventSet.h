#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIEvent.h"
#include "CEGUIIteratorBase.h"
#include <map>

namespace CEGUI
{

class EventSet
{
public:
	EventSet();
	//virtual类型，可被继承
	virtual ~EventSet(void);
	//exception AlreadyExistsException	Thrown if an Event already exists named \a name.
	void	addEvent(const String& name);
	void	removeEvent(const String& name);
	void	removeAllEvents(void);
	bool	isEventPresent(const String& name);
	//virtual类型，事件注册的具体操作是由Event对象来执行
    virtual Event::Connection subscribeEvent(const String& name, Event::Subscriber subscriber);
    virtual Event::Connection subscribeEvent(const String& name, Event::Group group, Event::Subscriber subscriber);
    virtual Event::Connection subscribeScriptedEvent(const String& name, const String& subscriber_name);
    virtual Event::Connection subscribeScriptedEvent(const String& name, Event::Group group, const String& subscriber_name);
    virtual void fireEvent(const String& name, EventArgs& args, const String& eventNamespace = "");
	bool	isMuted(void) const;
	void	setMutedState(bool setting);
protected:
    Event* getEventObject(const String& name, bool autoAdd = false);
    void fireEvent_impl(const String& name, EventArgs& args);
	// Do not allow copying, assignment, or any other usage than simple creation.
	EventSet(EventSet&) {}
	EventSet& operator=(EventSet&) {return *this;}

	typedef std::map<String, Event*, String::FastLessCompare>	EventMap;
	EventMap	d_events;

	bool	d_muted;	//!< true if events for this EventSet have been muted.

public:
    typedef	ConstBaseIterator<EventMap> Iterator;
    Iterator getIterator(void) const;
};

}