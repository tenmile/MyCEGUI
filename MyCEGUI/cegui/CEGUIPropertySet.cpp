#include "CEGUIPropertySet.h"
#include "CEGUIProperty.h"
#include "CEGUIExceptions.h"

namespace CEGUI
{

/*************************************************************************
	Add a new property to the set
*************************************************************************/
void PropertySet::addProperty(Property* property)
{
	if (!property)
	{
		CEGUI_THROW(NullObjectException("The given Property object pointer is invalid."));
	}

	if (d_properties.find(property->getName()) != d_properties.end())
	{
		CEGUI_THROW(AlreadyExistsException("A Property named '" + property->getName() + "' already exists in the PropertySet."));
	}

	d_properties[property->getName()] = property;
}

/*************************************************************************
	Remove a property from the set
*************************************************************************/
void PropertySet::removeProperty(const String& name)
{
	PropertyRegistry::iterator pos = d_properties.find(name);

	if (pos != d_properties.end())
	{
		d_properties.erase(pos);
	}
}

/*************************************************************************
	Remove all properties from the set
*************************************************************************/
void PropertySet::clearProperties(void)
{
	d_properties.clear();
}

/*************************************************************************
	Return true if a property with the given name is in the set
*************************************************************************/
bool PropertySet::isPropertyPresent(const String& name) const
{
	return (d_properties.find(name) != d_properties.end());
}

/*************************************************************************
	Return the help string for a property
*************************************************************************/
const String& PropertySet::getPropertyHelp(const String& name) const
{
	PropertyRegistry::const_iterator pos = d_properties.find(name);

	if (pos == d_properties.end())
	{
		CEGUI_THROW(UnknownObjectException("There is no Property named '" + name + "' available in the set."));
	}

	return pos->second->getHelp();
}

/*************************************************************************
	Return the current value of a property
*************************************************************************/
String PropertySet::getProperty(const String& name) const
{
	PropertyRegistry::const_iterator pos = d_properties.find(name);

	if (pos == d_properties.end())
	{
		CEGUI_THROW(UnknownObjectException("There is no Property named '" + name + "' available in the set."));
	}

	//传入的是当前窗口指针
	return pos->second->get(this);
}

/*************************************************************************
	Set the current value of a property
*************************************************************************/
void PropertySet::setProperty(const String& name,const String& value)
{
	PropertyRegistry::iterator pos = d_properties.find(name);

	if (pos == d_properties.end())
	{
		CEGUI_THROW(UnknownObjectException("There is no Property named '" + name + "' available in the set."));
	}
	//传入的是当前窗口指针
	pos->second->set(this, value);
}


/*************************************************************************
	Return a PropertySet::PropertyIterator object to iterate over the
	available Properties.
*************************************************************************/
PropertySet::Iterator PropertySet::getIterator(void) const
{
	return Iterator(d_properties.begin(), d_properties.end());
}


/*************************************************************************
	Returns whether a Property is at it's default value.
*************************************************************************/
bool PropertySet::isPropertyDefault(const String& name) const
{
	PropertyRegistry::const_iterator pos = d_properties.find(name);

	if (pos == d_properties.end())
	{
		CEGUI_THROW(UnknownObjectException("There is no Property named '" + name + "' available in the set."));
	}

	return pos->second->isDefault(this);
}


/*************************************************************************
	Returns the default value of a Property as a String.	
*************************************************************************/
String PropertySet::getPropertyDefault(const String& name) const
{
	PropertyRegistry::const_iterator pos = d_properties.find(name);

	if (pos == d_properties.end())
	{
		CEGUI_THROW(UnknownObjectException("There is no Property named '" + name + "' available in the set."));
	}

	return pos->second->getDefault(this);
}

}