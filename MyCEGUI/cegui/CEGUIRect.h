#pragma once

#include "CEGUIBase.h"
#include "CEGUIVector.h"
#include "CEGUISize.h"

namespace CEGUI
{
/*!
\brief
	Class encapsulating operations on a Rectangle
*/
class Rect
{
public:
	Rect(void) {}


	/*!
	\brief
		Constructor for a Rect.
	*/
	Rect(float left, float top, float right, float bottom);

    Rect(Point pos, Size sz);


	/*!
	\brief
		Return top-left postion of Rect as a Point
	*/
	Point	getPosition(void) const		{return Point(d_left, d_top);}

	/*!
	\brief
		return width of Rect area
	*/
	float	getWidth(void) const		{return d_right - d_left;}


	/*!
	\brief
		return height of Rect area
	*/
	float	getHeight(void) const		{return d_bottom - d_top;}


	/*!
	\brief
		return the size of the Rect area
	*/
	Size	getSize(void) const			{return Size(getWidth(), getHeight());}


	/*!
	\brief
		set the position of the Rect (leaves size in tact)
	*/
	void	setPosition(const Point& pt);


	/*!
	\brief
		set the width of the Rect object
	*/
	void	setWidth(float width)		{d_right = d_left + width;}

	/*!
	\brief
		set the height of the Rect object
	*/
	void	setHeight(float height)		{d_bottom = d_top + height;}


	/*!
	\brief
		set the size of the Rect area
	*/
	void	setSize(const Size& sze)	{setWidth(sze.d_width); setHeight(sze.d_height);}


	/*!
	\brief
		return a Rect that is the intersection of 'this' Rect with the Rect 'rect'

	\note
		It can be assumed that if d_left == d_right, or d_top == d_bottom, or getWidth() == 0, or getHeight() == 0, then
		'this' rect was totally outside 'rect'.
	*/
	Rect	getIntersection(const Rect& rect) const;


	/*!
	\brief
		Applies an offset the Rect object

	\param pt
		Point object containing the offsets to be applied to the Rect.

	\return
		this Rect after the offset is performed
	*/
	Rect&	offset(const Point& pt);


	/*!
	\brief
		Return true if the given Point falls within this Rect

	\param pt
		Point object describing the position to test.

	\return
		true if position \a pt is within this Rect's area, else false
	*/
	bool	isPointInRect(const Point& pt) const;


	/*!
	\brief
		check the size of the Rect object and if it is bigger than \a sz, resize it so it isn't.

	\param sz
		Size object that describes the maximum dimensions that this Rect should be limited to.

	\return
		'this' Rect object after the constrain operation
	*/
	Rect&	constrainSizeMax(const Size& sz);


	/*!
	\brief
		check the size of the Rect object and if it is smaller than \a sz, resize it so it isn't.

	\param sz
		Size object that describes the minimum dimensions that this Rect should be limited to.

	\return
		'this' Rect object after the constrain operation
	*/
	Rect&	constrainSizeMin(const Size& sz);


	/*!
	\brief
		check the size of the Rect object and if it is bigger than \a max_sz or smaller than \a min_sz, resize it so it isn't.

	\param max_sz
		Size object that describes the maximum dimensions that this Rect should be limited to.

	\param min_sz
		Size object that describes the minimum dimensions that this Rect should be limited to.

	\return
		'this' Rect object after the constrain operation
	*/
	Rect&	constrainSize(const Size& max_sz, const Size& min_sz);


	/*************************************************************************
		Operators
	*************************************************************************/
	bool	operator==(const Rect& rhs) const
	{
		return ((d_left == rhs.d_left) && (d_right == rhs.d_right) && (d_top == rhs.d_top) && (d_bottom == rhs.d_bottom));
	}

	bool	operator!=(const Rect& rhs) const		{return !operator==(rhs);}

	Rect&	operator=(const Rect& rhs);

    Rect operator*(float scalar) const      { return Rect(d_left * scalar, d_top * scalar, d_right * scalar, d_bottom * scalar); }
    const Rect& operator*=(float scalar)    { d_left *= scalar; d_top *= scalar; d_right *= scalar; d_bottom *= scalar; return *this; }

	Rect operator+(const Rect& r) const		{ return Rect(d_left + r.d_left, d_top + r.d_top, d_right + r.d_right, d_bottom + r.d_bottom); }


	/*************************************************************************
		Data Fields
	*************************************************************************/
	float	d_left, d_top, d_right, d_bottom;
};

}