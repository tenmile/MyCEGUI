#include "CEGUIXMLHandler.h"


namespace CEGUI
{
	// default handler does nothing.

	XMLHandler::XMLHandler(void)
	{}

	XMLHandler::~XMLHandler(void)
	{}

	void XMLHandler::elementStart(const String&, const XMLAttributes&)
	{}

	void XMLHandler::elementEnd(const String&)
	{}

	void XMLHandler::text(const String&)
	{}


}