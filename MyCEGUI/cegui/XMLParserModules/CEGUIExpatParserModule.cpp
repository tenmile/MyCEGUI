#include "CEGUIExpatParserModule.h"
#include "CEGUIExpatParser.h"

CEGUI::XMLParser* createParser(void)
{
	return new CEGUI::ExpatParser();
}

void destroyParser(CEGUI::XMLParser* parser)
{
	delete parser;
}