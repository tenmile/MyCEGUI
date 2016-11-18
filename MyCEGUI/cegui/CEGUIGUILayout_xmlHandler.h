#pragma once

#include "CEGUIWindowManager.h"
#include "CEGUIWindow.h"
#include "CEGUIXMLHandler.h"

#include <vector>

namespace CEGUI
{

//Handler class used to parse the GUILayout XML files using SAX2
class GUILayout_xmlHandler : public XMLHandler
{
	typedef WindowManager::PropertyCallback PropertyCallback;
public:
	/*************************************************************************
		Construction & Destruction
	*************************************************************************/
	GUILayout_xmlHandler(const String& name_prefix, PropertyCallback* callback = 0, void* userdata = 0) :
	  d_root(0),
	  d_namingPrefix(name_prefix),
	  d_propertyCallback(callback),
	  d_userData(userdata)
	  {}
	//virtual
	virtual ~GUILayout_xmlHandler(void) {}

    virtual void elementStart(const String& element, const XMLAttributes& attributes);
    virtual void elementEnd(const String& element);
    virtual void text(const String& text);

	/*************************************************************************
		Functions used by our implementation
	*************************************************************************/
	//销毁目前创建的所有窗口
	void	cleanupLoadedWindows(void);
	//返回根窗口
	Window*	getLayoutRootWindow(void) const;

private:
	/*************************************************************************
		Implementation Constants
	*************************************************************************/
	static const String GUILayoutElement;				//!< Tag name for GUILayout elements.
	static const String WindowElement;				//!< Tag name for Window elements.
    static const String AutoWindowElement;              //!< Tag name for AutoWindow elements.
	static const String PropertyElement;				//!< Tag name for Property elements.
	static const String LayoutImportElement;			//!< Tag name for LayoutImport elements.
	static const String EventElement;					//!< Tag name for Event elements.
	static const String WindowTypeAttribute;			//!< Attribute name that stores the type of Window to create.
	static const String WindowNameAttribute;			//!< Attribute name that stores the name of the window to create.
    static const String AutoWindowNameSuffixAttribute;  //!< Attribute name that stores the name suffix of the auto window to get.
	static const String PropertyNameAttribute;		//!< Attribute name that stores the name of the property to set.
	static const String PropertyValueAttribute;		//!< Attribute name that stores the value to pass to the property.
	static const String LayoutParentAttribute;		//!< Attribute name that stores the name of the window to attach the layout to.
	static const String LayoutImportFilenameAttribute;//!< Attribute name that stores the file name of the layout to import.
	static const String LayoutImportPrefixAttribute;	//!< Attribute name that stores the prefix to use when loading the imported layout.
    static const String LayoutImportResourceGroupAttribute; //!< Attribute name that stores the resource group identifier used when loading imported file.
	static const String EventNameAttribute;			//!< Attribute name that stores the event name to be subscribed.
	static const String EventFunctionAttribute;		//!< Attribute name that stores the name of the scripted function to be bound.
    void elementGUILayoutStart(const XMLAttributes& attributes);
    void elementWindowStart(const XMLAttributes& attributes);
    void elementAutoWindowStart(const XMLAttributes& attributes);
    void elementPropertyStart(const XMLAttributes& attributes);
    void elementLayoutImportStart(const XMLAttributes& attributes);
    void elementEventStart(const XMLAttributes& attributes);
    void elementGUILayoutEnd();
    void elementWindowEnd();
    void elementAutoWindowEnd();
    void elementPropertyEnd();

    void operator=(const GUILayout_xmlHandler&) {}

    /*************************************************************************
		Implementation Data
	*************************************************************************/
    typedef std::pair<Window*,bool> WindowStackEntry; //!< Pair used as datatype for the window stack. second is false if the window is an autowindow.
	typedef std::vector<WindowStackEntry>	WindowStack;
	Window*	d_root;				//!< Will point to first window created.
	WindowStack	d_stack;		//!< Stack used to keep track of what we're doing to which window.
	String		d_layoutParent;	//!< Name of the parent window to attach the loaded layout to.
	const String&		d_namingPrefix;	//!< Prefix that is to prepend all names of created windows.
	PropertyCallback*	d_propertyCallback; //!< Callback for every property loaded
	void*				d_userData;			//!< User data for the property callback
	String d_propertyName; //!< Use for long property value 
	String d_propertyValue; //!< Use for long property value 
};

}