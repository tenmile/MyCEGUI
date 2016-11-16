#pragma once

#include <cassert>
#include <string>

#include "CEGUIConfig.h"

#include "CEGUIVersion.h"

/*************************************************************************
	Bring in forward references to all GUI base system classes
*************************************************************************/
#include "CEGUIForwardRefs.h"



#define ceguimin	std::min
#define ceguimax	std::max

#ifndef CEGUI_TRY
#   define CEGUI_TRY try
#endif
#ifndef CEGUI_CATCH
#   define CEGUI_CATCH(e) catch (e)
#endif
#ifndef CEGUI_THROW
#   define CEGUI_THROW(e) throw e
#endif
#ifndef CEGUI_RETHROW
#   define CEGUI_RETHROW throw
#endif


//////////////////////////////////////////////////////////////////////////
// Comment this line to remove the alignment of elements to pixel
// boundaries.  This may give you a performance boost at the expense
// of visual quality
//////////////////////////////////////////////////////////////////////////
#define CEGUI_ALIGN_ELEMENTS_TO_PIXELS 1

/*!
\brief
	Macro used to return a float value rounded to the nearest integer.

	This macro is used throughout the library to ensure that elements are
	kept at integer pixel positions on the display.

\param x
	Expression to be rounded to nearest whole number

\return
	\a x after having been rounded
*/
#if defined(CEGUI_ALIGN_ELEMENTS_TO_PIXELS)
#	define PixelAligned(x)	( (float)(int)(( x ) + (( x ) > 0.0f ? 0.5f : -0.5f)) )
#else
#	define PixelAligned(x)	( x )
#endif



/*************************************************************************
	Documentation for the CEGUI namespace itself
*************************************************************************/
namespace CEGUI
{

/*************************************************************************
	Simplification of some 'unsigned' types
*************************************************************************/
typedef	unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned char	uchar;

typedef unsigned int    uint32;
typedef unsigned short  uint16;
typedef unsigned char   uint8;


/*************************************************************************
	System wide constants
*************************************************************************/
static const float		DefaultNativeHorzRes	= 640.0f;		//!< Default native horizontal resolution (for fonts and imagesets)
static const float		DefaultNativeVertRes	= 480.0f;		//!< Default native vertical resolution (for fonts and imagesets)


/*************************************************************************
    Additional typedefs
*************************************************************************/
typedef std::ostream OutStream;     //!< Output stream class.
}




