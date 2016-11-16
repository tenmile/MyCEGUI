#include "CEGUIDataContainer.h"


namespace CEGUI
{

void RawDataContainer::release(void)
{
	if(mData)
	{
		delete [] mData;
		mData = 0;
	}
}

}