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

//�����ֺ���ָ���Լ��º�����װ����
class SubscriberSlot
{
public:
    SubscriberSlot();

	//�����ɺ���ָ���ʼ��
    SubscriberSlot(FreeFunctionSlot::SlotFunction* func);
	//����virtual���ͣ���Ҫ������
    ~SubscriberSlot();
    bool operator()(const EventArgs& args) const
    {
        return (*d_functor_impl)(args);
    }
	//�жϰ󶨵ĺ���ָ���Ƿ���Ч
    bool connected() const
    {
        return d_functor_impl != 0;
    }
    //�ͷź���ָ����ָ��Ķ���
    void cleanup();
	//����ĳ�Ա������ʼ��
    template<typename T>
    SubscriberSlot(bool (T::*function)(const EventArgs&), T* obj) :
        d_functor_impl(new MemberFunctionSlot<T>(function, obj))
    {}
	//��װ��ĳ��������������ã�FunctorRefrenceBinderֻ��Ϊ�˶Բ�����������
    template<typename T>
    SubscriberSlot(const FunctorReferenceBinder<T>& binder) :
        d_functor_impl(new FunctorReferenceSlot<T>(binder.d_functor))
    {}
	//��װ��ĳ����������Ŀ���
    template<typename T>
    SubscriberSlot(const T& functor) :
        d_functor_impl(new FunctorCopySlot<T>(functor))
    {}
	//�Ժ���ָ�����ķ�װ
    template<typename T>
    SubscriberSlot(T* functor) :
        d_functor_impl(new FunctorPointerSlot<T>(functor))
    {}

private:
    //ָ���������ָ��
    SlotFunctorBase* d_functor_impl;
};

}