#pragma once

#include "CEGUIBase.h"

namespace CEGUI
{

/*!
\brief
	Class that holds the size (width & height) of something.
*/
class Size
{
public:
	Size(void) {}
	Size(float width, float height) : d_width(width), d_height(height) {}
	Size(const Size& v): d_width(v.d_width), d_height(v.d_height) {}

	bool operator==(const Size& other) const;
	bool operator!=(const Size& other) const;

	float d_width, d_height;

	Size operator*(float c) const
	{
		return Size(d_width * c, d_height * c);
	}

	Size operator+(const Size& s) const
	{
		return Size(d_width + s.d_width, d_height + s.d_height);
	}
};

}