#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIXMLSerializer.h" 

namespace CEGUI
{

//只作为基类存在
class PropertyReceiver
{
public:
	PropertyReceiver() {}
	virtual ~PropertyReceiver() {}
};

class Property
{
public:
	Property(const String& name, const String& help, const String& defaultValue = "", bool writesXML = true) :
	  d_name(name),
	  d_help(help),
	  d_default(defaultValue),
	  d_writeXML(writesXML)
	{
	}
	//virtual
	virtual ~Property(void) {}
	const String& getHelp(void) const		{return d_help;}
	const String& getName(void) const		{return d_name;}
	virtual String	get(const PropertyReceiver* receiver) const = 0;
	//exception InvalidRequestException	Thrown when the Property was unable to interpret the content of \a value.
	virtual void	set(PropertyReceiver* receiver, const String& value) = 0;
	virtual bool	isDefault(const PropertyReceiver* receiver) const;
	virtual String	getDefault(const PropertyReceiver* receiver) const;
    virtual void writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const;

protected:
	String	d_name;		//!< String that stores the Property name.
	String	d_help;		//!< String that stores the Property help text.
	String	d_default;	//!< String that stores the Property default value string.
	bool    d_writeXML; //!< Specifies whether writeXMLToStream should do anything for this property.
};

}