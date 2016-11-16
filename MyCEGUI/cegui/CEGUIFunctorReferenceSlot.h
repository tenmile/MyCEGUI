#pragma once

#include "CEGUISlotFunctorBase.h"

namespace CEGUI
{

template<typename T>
class FunctorReferenceSlot : public SlotFunctorBase
{
public:
    FunctorReferenceSlot(T& functor) :
        d_functor(functor)
    {}

    virtual bool operator()(const EventArgs& args)
    {
        return d_functor(args);
    }

private:
    T& d_functor;
};

}