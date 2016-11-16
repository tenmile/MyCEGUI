#include "CEGUIEventSet.h"
#include "CEGUIExceptions.h"
#include "CEGUIGlobalEventSet.h"
#include "CEGUIScriptModule.h"
#include "CEGUISystem.h"

namespace CEGUI
{

EventSet::EventSet() :
	d_muted(false)
{
}

EventSet::~EventSet(void)
{
	removeAllEvents();
}

void EventSet::addEvent(const String& name)
{
	if (isEventPresent(name))
	{
		CEGUI_THROW(AlreadyExistsException("An event named '" + name + "' already exists in the EventSet."));
	}

	d_events[name] = new Event(name);
}

void EventSet::removeEvent(const String& name)
{
	EventMap::iterator pos = d_events.find(name);

	if (pos != d_events.end())
	{
		delete pos->second;
		d_events.erase(pos);
	}

}

void EventSet::removeAllEvents(void)
{
	EventMap::iterator pos = d_events.begin();
	EventMap::iterator end = d_events.end()	;

	for (; pos != end; ++pos)
	{
		delete pos->second;     //×¢ÒâÄÚ´æÐ¹Â©
	}

	d_events.clear();
}

bool EventSet::isEventPresent(const String& name)
{
	return (d_events.find(name) != d_events.end());
}


Event::Connection EventSet::subscribeScriptedEvent(const String& name, const String& subscriber_name)
{
	//return subscribeEvent(name, Event::Subscriber(ScriptFunctor(subscriber_name)));
	ScriptModule* sm = System::getSingletonPtr()->getScriptingModule();
	if (!sm)
	{
	   CEGUI_THROW(InvalidRequestException("[EventSet::subscribeScriptedEvent] No scripting module is available"));
	}
	return sm->subscribeEvent(this, name, subscriber_name);
}

Event::Connection EventSet::subscribeScriptedEvent(const String& name, Event::Group group, const String& subscriber_name)
{
    //return subscribeEvent(name, group, Event::Subscriber(ScriptFunctor(subscriber_name)));
    ScriptModule* sm = System::getSingletonPtr()->getScriptingModule();
	if (!sm)
	{
	   CEGUI_THROW(InvalidRequestException("[EventSet::subscribeScriptedEvent] No scripting module is available"));
	}
	return sm->subscribeEvent(this, name, group, subscriber_name);
}


Event::Connection EventSet::subscribeEvent(const String& name, Event::Subscriber subscriber)
{
    // do subscription & return connection
    return getEventObject(name, true)->subscribe(subscriber);
}

Event::Connection EventSet::subscribeEvent(const String& name, Event::Group group, Event::Subscriber subscriber)
{
    // do subscription with group & return connection
    return getEventObject(name, true)->subscribe(group, subscriber);
}

void EventSet::fireEvent(const String& name, EventArgs& args, const String& eventNamespace)
{
    // handle global events
    GlobalEventSet::getSingleton().fireEvent(name, args, eventNamespace);
    // handle local event
    fireEvent_impl(name, args);
}

bool EventSet::isMuted(void) const
{
	return d_muted;
}


void EventSet::setMutedState(bool setting)
{
	d_muted = setting;
}

Event* EventSet::getEventObject(const String& name, bool autoAdd)
{
    EventMap::iterator pos = d_events.find(name);

    // if event did not exist, add it and then find it.
    if (pos == d_events.end())
    {
        if (autoAdd)
        {
            addEvent(name);
            return d_events.find(name)->second;
        }
        else
        {
            return 0;
        }
    }

    return pos->second;
}


void EventSet::fireEvent_impl(const String& name, EventArgs& args)
{
    // find event object
    Event* ev = getEventObject(name);

    // fire the event if present and set is not muted
    if ((ev != 0) && !d_muted)
        (*ev)(args);
}

EventSet::Iterator EventSet::getIterator(void) const
{
	return Iterator(d_events.begin(), d_events.end());
}


}