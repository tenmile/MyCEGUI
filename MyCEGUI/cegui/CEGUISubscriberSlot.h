#pragma once

#include "CEGUIBase.h"
#include "CEGUIFreeFunctionSlot.h"
#include "CEGUIFunctorCopySlot.h"
#include "CEGUIFunctorReferenceSlot.h"
#include "CEGUIFunctorPointerSlot.h"
#include "CEGUIMemberFunctionSlot.h"
#include "CEGUIFunctorReferenceBinder.h"

namespace CEGUI
{

//将各种函数指针以及仿函数封装起来
class SubscriberSlot
{
public:
    SubscriberSlot();

	//用自由函数指针初始化
    SubscriberSlot(FreeFunctionSlot::SlotFunction* func);
	//不是virtual类型，不要有子类
    ~SubscriberSlot();
    bool operator()(const EventArgs& args) const
    {
        return (*d_functor_impl)(args);
    }
	//判断绑定的函数指针是否有效
    bool connected() const
    {
        return d_functor_impl != 0;
    }
    //释放函数指针所指向的对象
    void cleanup();
	//用类的成员函数初始化
    template<typename T>
    SubscriberSlot(bool (T::*function)(const EventArgs&), T* obj) :
        d_functor_impl(new MemberFunctionSlot<T>(function, obj))
    {}
	//封装成某个函数对象的引用，FunctorRefrenceBinder只是为了对参数进行区分
    template<typename T>
    SubscriberSlot(const FunctorReferenceBinder<T>& binder) :
        d_functor_impl(new FunctorReferenceSlot<T>(binder.d_functor))
    {}
	//封装成某个函数对象的拷贝
    template<typename T>
    SubscriberSlot(const T& functor) :
        d_functor_impl(new FunctorCopySlot<T>(functor))
    {}
	//对函数指针对象的封装
    template<typename T>
    SubscriberSlot(T* functor) :
        d_functor_impl(new FunctorPointerSlot<T>(functor))
    {}

private:
    //指向函数对象的指针
    SlotFunctorBase* d_functor_impl;
};

}