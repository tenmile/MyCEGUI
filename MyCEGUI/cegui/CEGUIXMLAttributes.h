#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include <map>


namespace CEGUI
{

class XMLAttributes
{
public:
    XMLAttributes(void);
	//virtual
    virtual ~XMLAttributes(void);
	//存在，则替换为新值
    void add(const String& attrName, const String& attrValue);
    void remove(const String& attrName);
    bool exists(const String& attrName) const;
    size_t getCount(void) const;
    //exception IllegalRequestException  thrown if \a index is out of range for this attribute block.
    const String& getName(size_t index) const;
    //exception IllegalRequestException  thrown if \a index is out of range for this attribute block.
    const String& getValue(size_t index) const;
    //exception UnknownObjectException   thrown if no attribute named \a attrName is present in the attribute block.
    const String& getValue(const String& attrName) const;
    const String& getValueAsString(const String& attrName, const String& def = "") const;
    //exception IllegalRequestException  thrown if the attribute value string coul dnot be converted to the requested type.
    bool getValueAsBool(const String& attrName, bool def = false) const; 
    //exception IllegalRequestException  thrown if the attribute value string coul dnot be converted to the requested type.
    int getValueAsInteger(const String& attrName, int def = 0) const;
    //exception IllegalRequestException  thrown if the attribute value string coul dnot be converted to the requested type.
    float getValueAsFloat(const String& attrName, float def = 0.0f) const;

protected:
    typedef std::map<String, String, String::FastLessCompare> AttributeMap;
    AttributeMap    d_attrs;
};

}