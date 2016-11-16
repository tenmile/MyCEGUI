#pragma once

#include "CEGUIString.h"
#include "CEGUIBoundSlot.h"
#include "CEGUISubscriberSlot.h"
#include "CEGUIRefCounted.h"

#include <map>

namespace CEGUI
{

//һ���¼��ɰ󶨶�������������
class Event
{
public:
	//��BoundSlot�ļ򵥷�װ�࣬�ɶ��¼�����ע���Լ���ע��
    typedef RefCounted<BoundSlot> Connection;  //�������ü������¼�������
	//��ʷ�ͼ�����ԭ��
    typedef CEGUI::SubscriberSlot Subscriber;
    typedef unsigned int Group;

    //�¼���������ʱ�����Զ��Ͽ����¼������ӣ��̳�Connection�к����壿
    class ScopedConnection : public Connection  //Connection�İ�װ��
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

	//���¼�������һ���µ��¼�����Ψһ�Ĺ��캯��
    Event(const String& name);
	//ע���Ƿ�virtual���ͣ���Ҫ������
	//�����е��¼������������ͷţ�������¼�����
    ~Event();
    const String& getName(void) const
        { return d_name; }
	//Connection�����������ע��
    Connection subscribe(const Subscriber& slot);
	//����group��ע�ắ��
    Connection subscribe(Group group, const Subscriber& slot);
	//�������е��¼��������
    void operator()(EventArgs& args);


private:
    friend void CEGUI::BoundSlot::disconnect();  //������ע��
    //��ע���¼�
    void unsubscribe(const BoundSlot& slot);

    // Copy constructor and assignment are not allowed for events
    Event(const Event&) {}
    Event& operator=(const Event&)  {return *this;}

    typedef std::multimap<Group, Connection> SlotContainer;
    SlotContainer d_slots;  //!< Collection holding ref-counted bound slots
    const String d_name;    //!< Name of this event
};

}