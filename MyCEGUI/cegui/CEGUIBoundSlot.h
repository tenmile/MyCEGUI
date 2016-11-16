#include "CEGUIBase.h"
#include "CEGUISubscriberSlot.h"

namespace CEGUI
{

//将事件处理对象指针，时间指针，组封装在一起
class BoundSlot
{
public:
    typedef unsigned int Group;
	//构造新的SubscriberSlot对象进行初始化
    BoundSlot(Group group, const SubscriberSlot& subscriber, Event& event);
	//构造新的SubscriberSlot对象进行复制，即SubscriberSlot对象用完后可被释放
    BoundSlot(const BoundSlot& other);
	//调用disconnect进行析构，并delete d_subscriber所指向的对象
    ~BoundSlot();
	//是否绑定了相应的事件处理函数
    bool connected() const;
	//释放处理函数对象，并用event指针对此对象进行反注册
    void disconnect();
	//只判断相关事件处理函数对象是否相等
    bool operator==(const BoundSlot& other) const;
    bool operator!=(const BoundSlot& other) const;

private:
    friend class Event;
    // no assignment.
    BoundSlot& operator=(const BoundSlot& other);
    Group d_group;                  //! The group the slot subscription used.
    SubscriberSlot* d_subscriber;   //! The actual slot object.
    Event* d_event;                 //! The event to which the slot was attached，可进行反注册
};

}