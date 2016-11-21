#include "CEGUIExpatParser.h"
#include "CEGUIResourceProvider.h"
#include "CEGUISystem.h"
#include "CEGUIXMLHandler.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUIExceptions.h"
#include "CEGUIPropertyHelper.h"
#include <expat.h>

namespace CEGUI
{
	ExpatParser::ExpatParser(void)
	{
		// set ID string
		d_identifierString = "CEGUI::ExpatParser - Official expat based parser module for CEGUI";
	}

	ExpatParser::~ExpatParser(void)
	{
	}

	void ExpatParser::parseXMLFile(XMLHandler& handler, const String& filename, const String& /*schemaName*/, const String& resourceGroup)
	{
		// All stuff goes here
		XML_Parser parser = XML_ParserCreate(0); // Create a parser

		if (! parser)
		{
			CEGUI_THROW(GenericException("ExpatParser::parseXMLFile - Unable to create a new Expat Parser"));
		}

		XML_SetUserData(parser, (void*)&handler); // Initialise user data
		XML_SetElementHandler(parser, startElement, endElement); // Register callback for elements
		XML_SetCharacterDataHandler(parser, characterData); // Register callback for character data

		// Aquire resource using CEGUI ResourceProvider
		CEGUI::RawDataContainer rawXMLData;
		CEGUI::System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, rawXMLData, resourceGroup);

		// Parse the data (note that the last true parameter tels Expat that this is the last chunk of the document
		if ( ! XML_Parse(parser, reinterpret_cast<const char*>(rawXMLData.getDataPtr()), rawXMLData.getSize(), true))
		{
			System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);
			String exception (String((const utf8*)"ExpatParser::parseXMLFile - XML Parsing error '") +
				String((const utf8*)XML_ErrorString(XML_GetErrorCode(parser))) +
				String((const utf8*)"' at line ") +
				PropertyHelper::uintToString(XML_GetCurrentLineNumber(parser)));
			// (We know it is a valid pointer, otherwise an exception would have been thrown above.)
			XML_ParserFree(parser);
			CEGUI_THROW(GenericException(exception));
		}

		// Release resource
		CEGUI::System::getSingleton().getResourceProvider()->unloadRawDataContainer(rawXMLData);
		// (We know it is a valid pointer, otherwise an exception would have been thrown above.)
		XML_ParserFree(parser);
	}

	bool ExpatParser::initialiseImpl(void)
	{
		return true;
	}

	void ExpatParser::cleanupImpl(void)
	{

	}

	void ExpatParser::startElement(void* data, const char* element, const char** attr)
	{
		XMLHandler* handler = static_cast<XMLHandler*>(data);
		XMLAttributes attrs;

		for(size_t i = 0 ; attr[i] ; i += 2)
			attrs.add((const utf8*)attr[i], (const utf8*)attr[i+1]);

		handler->elementStart((const utf8*)element, attrs);
	}

	void ExpatParser::endElement(void* data, const char* element)
	{
		XMLHandler* handler = static_cast<XMLHandler*>(data);
		handler->elementEnd((const utf8*)element);
	}

	void ExpatParser::characterData(void *data, const char *text, int len)
	{
		XMLHandler* handler = static_cast<XMLHandler*>(data);
		String str((const utf8*)text, static_cast<String::size_type>(len));
		handler->text(str);
	}

}