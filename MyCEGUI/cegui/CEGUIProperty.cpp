#include "CEGUIProperty.h"
#include <iostream>


namespace CEGUI
{
/*************************************************************************
	Returns whether the property is at it's default value.
*************************************************************************/
bool Property::isDefault(const PropertyReceiver* receiver) const
{
	return (get(receiver) == d_default);
}

/*************************************************************************
	Returns the default value of the Property as a String.
*************************************************************************/
String Property::getDefault(const PropertyReceiver*) const
{
	return d_default;
}

void Property::writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const
{
    if (d_writeXML)
    {
        xml_stream.openTag("Property")
            .attribute("Name",  d_name);
        // Detect wether it is a long property or not 
        // Long property are needed if 
        const String& value = get(receiver);
        if (value.find((utf32)'\n') != String::npos)
        {
            xml_stream.text(value);
        }
        else 
        {
            xml_stream.attribute("Value", get(receiver));
        }
        xml_stream.closeTag();
    }
}

}