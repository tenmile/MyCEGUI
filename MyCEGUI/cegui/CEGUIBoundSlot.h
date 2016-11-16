#include "CEGUIBase.h"
#include "CEGUISubscriberSlot.h"

namespace CEGUI
{

//���¼��������ָ�룬ʱ��ָ�룬���װ��һ��
class BoundSlot
{
public:
    typedef unsigned int Group;
	//�����µ�SubscriberSlot������г�ʼ��
    BoundSlot(Group group, const SubscriberSlot& subscriber, Event& event);
	//�����µ�SubscriberSlot������и��ƣ���SubscriberSlot���������ɱ��ͷ�
    BoundSlot(const BoundSlot& other);
	//����disconnect������������delete d_subscriber��ָ��Ķ���
    ~BoundSlot();
	//�Ƿ������Ӧ���¼�������
    bool connected() const;
	//�ͷŴ��������󣬲���eventָ��Դ˶�����з�ע��
    void disconnect();
	//ֻ�ж�����¼������������Ƿ����
    bool operator==(const BoundSlot& other) const;
    bool operator!=(const BoundSlot& other) const;

private:
    friend class Event;
    // no assignment.
    BoundSlot& operator=(const BoundSlot& other);
    Group d_group;                  //! The group the slot subscription used.
    SubscriberSlot* d_subscriber;   //! The actual slot object.
    Event* d_event;                 //! The event to which the slot was attached���ɽ��з�ע��
};

}