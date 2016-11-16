#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include <vector>

namespace CEGUI
{
    /*!
    \brief 
         Class used to create XML Document. 

    This class hides the complexity of formatting valid XML files. The
    class provides automatic substitution of entities, XML indenting
    in respect of the spaces. It does not contains any codes specific
    to CEGUI taking appart the CEGUI::String class. The following
    example show the code needed to exports parts of an XML document
    similar to what can be found in a layout.
    
    @code 
    #include <iostream>
    #include <CEGUI/XMLSerializer.h> 
    
    int main() 
    {
       // Create an encoder that outputs its result on standard output 
       XMLSerializer xml(std::cout, 4);
       xml.openTag("Window")
          .attribute("Type", "TaharezLook/StaticText")
          .attribute("Name", "Test")
          .openTag("Property")
          .attribute("Name", "Text")
          .text("This is the static text to be displayed")
          .closeTag()
          .openTag("Window")
          .attribute("Name", "Button")
          .attribute("Type", "Vanilla/Button")
          .openTag("Property")
          .attribute("Name", "Text")
          .attribute("Value", "Push me")
          .closeTag()
          .closeTag()
          .closeTag();
          
       if (xml)
       {
           std::cout << "XML Exported successfully" << std::endl;
       }
       return 0;
    }
    @endcode
    */
class XMLSerializer
{
public:
    /*!
    \brief XMLSerializer constructor 
         
    \param out The stream to use to export the result 
         
         
    \param indentSpace The indentation level (0 to disable indentation)
    */
    XMLSerializer(OutStream& out, size_t indentSpace = 4);
        
    /*! 
    \brief XMLSerializer destructor
    */ 
    virtual ~XMLSerializer(void);  

    /*!
    \brief Start a new tag in the xml document.  
        
    \param name The name of the tag 
        
    \return 
        A reference to the current object for chaining operation 
    */
    XMLSerializer& openTag(const String& name);
    /*!
    \brief Close the current tag. 
        
    \return 
        A reference to the current object for chaining operation 
    */
    XMLSerializer& closeTag(void);
    /*!
    \brief After an opening tag you can populate attribute list with this function 
        
    \param name The name of the attribute 
        
    \param value The value of the attribute
        
    \return 
        A reference to the current object for chaining operation 
    */
    XMLSerializer& attribute(const String& name, const String& value);
    /*! 
    \brief Create a text node 
        
    \param text the content of the node 
        
    \return 
        A reference to the current object for chaining operation 
    */
    XMLSerializer& text(const String& text);
        
    /*! 
    \brief 
    report the nimber of tags created in the document 

    \return 
        return the number of tag created in the document 
    */
    unsigned int getTagCount() const;
    /*!
    \brief Check wether the XML Serializer status is valid 
        
    \return 
        True if all previous operations where successfull 
    */
    operator bool () const
    {
        return false == d_error;
    }
    /*!
    \brief Check wether the XML Serializer status is invalid 
        
    \return 
        True if one operations failed
    */ 
    bool operator!() const
    {
        return false != d_error;
    }
        
protected:
private:
    bool d_error; //!< Store the status of the serializer 
    unsigned int d_tagCount; //!<Return the number of tag in the document 
    size_t d_depth; //!< Store the current depth for indentation purpose 
    size_t d_indentSpace; //!< Store the number of space use for indenting 
    bool d_needClose; //!< Store whether the next operation need to close the tag or not 
    bool d_lastIsText; //!< Store whether the last operation was a text node or not 
    OutStream& d_stream; //!< A reference to the stream object use
    std::vector<String> d_tagStack; //!< Store the tag stack for correct closing of the tags. 
  
    /*!
    \brief put padding in the stream before line data 
    */
    void indentLine();
    /*!
    \brief convert special char to there corresponding entity in text data. 
    */
    static String convertEntityInText(const String& text);
    /*!
    \brief convert special char into entities including line ending for use in attributes.
    */
    static String convertEntityInAttribute(const String& attributeValue);
        

    // Disabled operation 
    XMLSerializer(const XMLSerializer& obj);
    // Disabled operation 
    XMLSerializer& operator=(const XMLSerializer& obj);  
};

}