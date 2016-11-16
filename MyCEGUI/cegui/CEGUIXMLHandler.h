#pragma once

#include "CEGUIBase.h"


// Start of CEGUI namespace section
namespace CEGUI
{

class XMLHandler
{
public:
    /*!
    \brief
        XMLHandler base class constructor.
        */
    XMLHandler(void);

    /*!
    \brief
        XMLHandler base class destructor.
        */
    virtual ~XMLHandler(void);
        
    /*!
    \brief
        Method called to notify the handler at the start of each XML element encountered.

    \param element
        String object holding the name of the element that is starting.

    \param attributes
        An XMLAttributes object holding the collection of attributes specified for the element.

    \return
        Nothing.
        */
    virtual void elementStart(const String& element, const XMLAttributes& attributes);
        
    /*!
    \brief
        Method called to notify the handler at the end of each XML element encountered.

    \param element
        String object holding the name of the element that is ending.

    \return
        Nothing.
        */
    virtual void elementEnd(const String& element);

/*! 
\brief
	Method called to notify text node, several successiv text node are agregated. 
	    
\param text 
	String object holding the content of the text node.

\return 
        Nothing.
*/
virtual void text(const String& text);
};

}