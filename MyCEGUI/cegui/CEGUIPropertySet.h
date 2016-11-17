#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIIteratorBase.h"
#include "CEGUIProperty.h"
#include <map>


namespace CEGUI
{

class PropertySet : public PropertyReceiver
{
public:
	PropertySet(void) {}
	//virtual
	virtual ~PropertySet(void) {}
	//exception NullObjectException		Thrown if \a property is NULL.
	//exception AlreadyExistsException	Thrown if a Property with the same name as \a property already exists in the PropertySet
	void	addProperty(Property* property);
	void	removeProperty(const String& name);
	void	clearProperties(void);
	bool	isPropertyPresent(const String& name) const;
	//exception UnknownObjectException	Thrown if no Property named \a name is in the PropertySet.
	const String&	getPropertyHelp(const String& name) const;
	//exception UnknownObjectException	Thrown if no Property named \a name is in the PropertySet.
	String	getProperty(const String& name) const;
	//exception UnknownObjectException	Thrown if no Property named \a name is in the PropertySet.
	//exception InvalidRequestException	Thrown when the Property was unable to interpret the content of \a value.
	void	setProperty(const String& name, const String& value);
	bool	isPropertyDefault(const String& name) const;
	String	getPropertyDefault(const String& name) const;

private:
	typedef std::map<String, Property*, String::FastLessCompare>	PropertyRegistry;
	PropertyRegistry	d_properties;


public:
	/*************************************************************************
		Iterator stuff
	*************************************************************************/
    typedef	ConstBaseIterator<PropertyRegistry> Iterator;
    Iterator getIterator(void) const;
};

}