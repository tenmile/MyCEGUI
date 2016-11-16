#pragma once

#include "CEGUISlotFunctorBase.h"

namespace CEGUI
{

class FreeFunctionSlot : public SlotFunctorBase
{
public:
	//! Slot function type.
	typedef bool (SlotFunction)(const EventArgs&);

	FreeFunctionSlot(SlotFunction* func) :
	d_function(func)
	{}

	virtual bool operator()(const EventArgs& args)
	{
		return d_function(args);
	}

private:
	SlotFunction* d_function;
};

}