#pragma once

#include "CEGUIBase.h"
#include "CEGUIVector.h"
#include "CEGUISize.h"

namespace CEGUI
{

//通过四条线来定义一个矩形
class Rect
{
public:
	Rect(void) {}
	Rect(float left, float top, float right, float bottom);
    Rect(Point pos, Size sz);
	//返回左上角的点
	Point	getPosition(void) const		{return Point(d_left, d_top);}
	float	getWidth(void) const		{return d_right - d_left;}
	float	getHeight(void) const		{return d_bottom - d_top;}
	Size	getSize(void) const			{return Size(getWidth(), getHeight());}
	void	setPosition(const Point& pt);
	void	setWidth(float width)		{d_right = d_left + width;}
	void	setHeight(float height)		{d_bottom = d_top + height;}
	void	setSize(const Size& sze)	{setWidth(sze.d_width); setHeight(sze.d_height);}
	Rect	getIntersection(const Rect& rect) const;
	//将矩形进行偏移
	Rect&	offset(const Point& pt);
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