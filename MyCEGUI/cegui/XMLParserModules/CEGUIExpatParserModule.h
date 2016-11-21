#pragma once

#include "CEGUIExpatParser.h"

/*!
\brief
    exported function that creates an XMLParser based object and returns
    a pointer to that object.
*/
extern "C" CEGUIEXPATPARSER_API CEGUI::XMLParser* createParser(void);

/*!
\brief
    exported function that deletes an XMLParser based object previously
    created by this module.
*/
extern "C" CEGUIEXPATPARSER_API void destroyParser(CEGUI::XMLParser* parser);