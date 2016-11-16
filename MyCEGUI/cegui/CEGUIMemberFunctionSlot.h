#pragma once

#include "CEGUISlotFunctorBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{

template<typename T>
class MemberFunctionSlot : public SlotFunctorBase
{
public:
    //! Member function slot type.
    typedef bool(T::*MemberFunctionType)(const EventArgs&);  //类成员函数指针，相当于类的偏移

    MemberFunctionSlot(MemberFunctionType func, T* obj) :
        d_function(func),
        d_object(obj)
    {}

    virtual bool operator()(const EventArgs& args)
    {
        return (d_object->*d_function)(args);
    }

private:
    MemberFunctionType d_function;
    T* d_object;
};

}