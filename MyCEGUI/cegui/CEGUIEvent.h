#pragma once

#include "CEGUIString.h"
#include "CEGUIBoundSlot.h"
#include "CEGUISubscriberSlot.h"
#include "CEGUIRefCounted.h"

#include <map>

namespace CEGUI
{

//一个事件可绑定多个函数处理对象
class Event
{
public:
	//对BoundSlot的简单封装类，可对事件进行注册以及反注册
    typedef RefCounted<BoundSlot> Connection;  //具有引用计数的事件处理类
	//历史和兼容性原因
    typedef CEGUI::SubscriberSlot Subscriber;
    typedef unsigned int Group;

    //事件对象被析构时，可自动断开和事件的连接，继承Connection有何意义？
    class ScopedConnection : public Connection  //Connection的包装类
    {
    public:
        ScopedConnection() {}

        ~ScopedConnection()
            { disconnect(); }

        ScopedConnection(const Event::Connection& connection) :
            d_connection(connection)
        {}

        ScopedConnection& operator=(const Event::Connection& connection)
        {
            d_connection = connection;
            return *this;
        }

        bool connected() const
            { return d_connection.isValid() ? d_connection->connected() : false; }

        void disconnect()
            { if (d_connection.isValid()) d_connection->disconnect(); }

    private:
        Event::Connection d_connection;
    };

	//用事件名创建一个新的事件对象，唯一的构造函数
    Event(const String& name);
	//注意是非virtual类型，不要有子类
	//将所有的事件处理函数对象释放，并清空事件集合
    ~Event();
    const String& getName(void) const
        { return d_name; }
	//Connection对象可用来反注册
    Connection subscribe(const Subscriber& slot);
	//带有group的注册函数
    Connection subscribe(Group group, const Subscriber& slot);
	//调用所有的事件处理对象
    void operator()(EventArgs& args);


private:
    friend void CEGUI::BoundSlot::disconnect();  //用来反注册
    //反注册事件
    void unsubscribe(const BoundSlot& slot);

    // Copy constructor and assignment are not allowed for events
    Event(const Event&) {}
    Event& operator=(const Event&)  {return *this;}

    typedef std::multimap<Group, Connection> SlotContainer;
    SlotContainer d_slots;  //!< Collection holding ref-counted bound slots
    const String d_name;    //!< Name of this event
};

}