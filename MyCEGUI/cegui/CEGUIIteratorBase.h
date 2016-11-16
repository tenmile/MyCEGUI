#pragma once

#include "CEGUIBase.h"


namespace CEGUI
{
/*!
\brief
	Base class constant iterator used to offer iteration over various collections within the system.
*/
template<class T>
class ConstBaseIterator
{
public:
	typedef typename T::mapped_type		mapped_type;

	/*!
	\brief
		ConstBaseIterator constructor

	\param start_iter
		'real' iterator that will be the start of the range to be iterated over by this iterator.

	\param end_iter
		'real' iterator that will be the end of the range to be iterated over by this iterator.
	*/
	ConstBaseIterator(typename T::const_iterator start_iter, typename T::const_iterator end_iter) :
		d_currIter(start_iter),
		d_startIter(start_iter),
		d_endIter(end_iter)
	{
	}

	
	/*!
	\brief
		ConstBaseIterator destructor
	*/
	~ConstBaseIterator(void)
	{
	}


	/*!
	\brief
		ConstBaseIterator copy constructor
	*/
	ConstBaseIterator(const ConstBaseIterator<T>& org) :
		d_currIter(org.d_currIter),
		d_startIter(org.d_startIter),
		d_endIter(org.d_endIter)
	{
	}


	/*!
	\brief
		ConstBaseIterator assignment operator
	*/
	ConstBaseIterator<T>&	operator=(const ConstBaseIterator<T>& rhs)
	{
		d_currIter	= rhs.d_currIter;
		d_startIter	= rhs.d_startIter;
		d_endIter	= rhs.d_endIter;

		return *this;
	}


	/*!
	\brief
		Return the key for the item at the current iterator position.
	*/
	typename T::key_type	getCurrentKey(void) const
	{
		return d_currIter->first;
	}


	/*!
	\brief
		Return the value for the item at the current iterator position.
	*/
	mapped_type	getCurrentValue(void) const
	{
		return d_currIter->second;
	}


	/*!
	\brief
		Return whether the current iterator position is at the end of the iterators range.
	*/
	bool	isAtEnd(void) const
	{
		return d_currIter == d_endIter;
	}


	/*!
	\brief
		Return whether the current iterator position is at the start of the iterators range.
	*/
	bool	isAtStart(void) const
	{
		return d_currIter == d_startIter;
	}


	/*!
	\brief
		Increase the iterator position (prefix increment).

	\note
		The iterator is checked, and this call will always succeed, so do not rely on some exception to exit a loop.
	*/
	ConstBaseIterator<T>&	operator++()
	{
		if (d_currIter != d_endIter)
			++d_currIter;

		return *this;
	}


	/*!
	\brief
		Increase the iterator position (postfix increment).

	\note
		The iterator is checked, and this call will always succeed, so do not rely on some exception to exit a loop.
	*/
	ConstBaseIterator<T>	operator++(int)
	{
		ConstBaseIterator<T> tmp = *this;
		++*this;

		return tmp;
	}


	/*!
	\brief
		Decrease the iterator position (prefix decrement).

	\note
		The iterator is checked, and this call will always succeed, so do not rely on some exception to exit a loop.
	*/
	ConstBaseIterator<T>&	operator--()
	{
		if (d_currIter != d_startIter)
			--d_currIter;

		return *this;
	}


	/*!
	\brief
		Decrease the iterator position (postfix decrement).

	\note
		The iterator is checked, and this call will always succeed, so do not rely on some exception to exit a loop.
	*/
	ConstBaseIterator<T>	operator--(int)
	{
		ConstBaseIterator<T> tmp = *this;
		--*this;

		return tmp;
	}


	/*!
	\brief
		Compares two iterators.  Return true if the current position of both iterators are equivalent.
	*/
	bool	operator==(const ConstBaseIterator<T>& rhs) const
	{
		return d_currIter == rhs.d_currIter;
	}


	/*!
	\brief
		Compares two iterators.  Return true if the current position of the iterators are different.
	*/
	bool	operator!=(const ConstBaseIterator<T>& rhs) const
	{
		return !operator==(rhs);
	}


	/*!
	\brief
		Return the value for the current iterator position.
	*/
	mapped_type	operator*() const
	{
		return d_currIter->second;
	}


	/*!
	\brief
		Set the iterator current position to the start position.
	*/
	void	toStart(void)
	{
		d_currIter = d_startIter;
	}


	/*!
	\brief
		Set the iterator current position to the end position.
	*/
	void	toEnd(void)
	{
		d_currIter = d_endIter;
	}


private:
	/*************************************************************************
		No default construction available
	*************************************************************************/
	ConstBaseIterator(void) {}

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typename T::const_iterator	d_currIter;		//!< 'real' iterator describing the current position within the collection.
	typename T::const_iterator	d_startIter;	//!< 'real' iterator describing the start position within the collection (or what we were told was the start).
	typename T::const_iterator	d_endIter;		//!< 'real' iterator describing the end position within the collection (or what we were told was the end).
};

}