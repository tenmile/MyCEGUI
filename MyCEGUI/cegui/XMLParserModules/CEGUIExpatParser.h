#pragma once

#include "../CEGUIXMLParser.h"

namespace CEGUI
{

/*!
\brief
    Implementation of XMLParser using Expat
*/
class ExpatParser : public XMLParser
{
public:
    ExpatParser(void);
    ~ExpatParser(void);
    // Implementation of public abstract interface
    void parseXMLFile(XMLHandler& handler, const String& filename, const String& schemaName, const String& resourceGroup);

protected:
    // Implementation of protected abstract interface.
    bool initialiseImpl(void);
    // Implementation of protected abstract interface.
    void cleanupImpl(void);
    // C++ class methods name are not valide C function pointer. static solve this
    static void startElement(void* data, const char* element, const char**attr); // Expat handlers
    static void endElement(void* data, const char* element); // Expat handlers
    static void characterData(void* data, const char* text, int len); // Expat handlers
};

}