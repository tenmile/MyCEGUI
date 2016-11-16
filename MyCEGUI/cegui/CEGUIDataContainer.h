#pragma once

#include "CEGUIBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Class used as the databuffer for loading files throughout the library.
*/
class RawDataContainer
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for RawDataContainer class
	*/
    RawDataContainer()
      : mData(0),
        mSize(0)
    {
    }

	/*!
	\brief
		Destructor for RawDataContainer class
	*/
	~RawDataContainer(void) 
    { 
        release();
    }

	/*************************************************************************
		Accessor functions
	*************************************************************************/
	/*!
	\brief
		Set a pointer to the external data.

	\param data
        Pointer to the uint8 data buffer.
	*/
    void setData(uint8* data) { mData = data; }

	/*!
	\brief
		Return a pointer to the external data

	\return
		Pointer to an the uint8 data buffer.
	*/
    uint8* getDataPtr(void) { return mData; }
    const uint8* getDataPtr(void) const { return mData; }

	/*!
	\brief
	    Set the size of the external data.

	\param size
	    size_t containing the size of the external data
	*/
    void setSize(size_t size) { mSize = size; }

	/*!
	\brief
		Get the size of the external data.

	\return
	    size_t containing the size of the external data
	*/
    size_t getSize(void) const { return mSize; }

	/*!
	\brief
		Release supplied data.
	*/
    void release(void);

private:
    /*************************************************************************
		Implementation Data
	*************************************************************************/
    uint8* mData;
    size_t mSize;
};

}