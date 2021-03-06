#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUILogger.h"
#include "CEGUIIteratorBase.h"
#include "CEGUIEventSet.h"
#include <map>
#include <vector>

namespace CEGUI
{
/*!
\brief
	The WindowManager class describes an object that manages creation and lifetime of Window objects.

	The WindowManager is the means by which Window objects are created and destroyed.  For each sub-class
	of Window that is to be created, there must exist a WindowFactory object which is registered with the
	WindowFactoryManager.  Additionally, the WindowManager tracks every Window object created, and can be
	used to access those Window objects by name.
*/
class WindowManager : public Singleton <WindowManager>,
                                  public EventSet
{
public:
    /*************************************************************************
        Public static data
    *************************************************************************/
    static const String GeneratedWindowNameBase;      //!< Base name to use for generated window names.
    //! Namespace for global events.
    static const String EventNamespace;
	//窗口创建事件
    static const String EventWindowCreated;
    //窗口销毁事件
    static const String EventWindowDestroyed;
	//加载layout时的回调函数，返回值表示属性是否应该被设置
	typedef bool PropertyCallback(Window* window, String& propname, String& propvalue, void* userdata);
	
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructs a new WindowManager object.

		NB: Client code should not create WindowManager objects - they are of limited use to you!  The
		intended pattern of access is to get a pointer to the GUI system's WindowManager via the System
		object, and use that.
	*/
	//所有的单例对象都有System创建
	WindowManager(void);


	/*!
	\brief
		Destructor for WindowManager objects

		This will properly destry all remaining Window objects.  Note that WindowFactory objects will not
		be destroyed (since they are owned by whoever created them).
	*/
	~WindowManager(void);


	/*************************************************************************
		Window Related Methods
	*************************************************************************/
    //exception  InvalidRequestException WindowManager is locked and no Windows may be created.
	//exception	AlreadyExistsException		A Window object with the name \a name already exists.
	//exception	UnknownObjectException		No WindowFactory is registered for \a type Window objects.
	//exception	GenericException			Some other error occurred (Exception message has details).
	Window* createWindow(const String& type, const String& name = "");
	//exception	InvalidRequestException		Can be thrown if the WindowFactory for \a window's object type was removed.
	void	destroyWindow(Window* window);
	//exception	InvalidRequestException		Can be thrown if the WindowFactory for \a window's object type was removed.
	void	destroyWindow(const String& window);
	//exception UnknownObjectException	No Window object with a name matching \a name was found.
	Window*	getWindow(const String& name) const;
	bool	isWindowPresent(const String& name) const;
	//exception	InvalidRequestException		Thrown if the WindowFactory for any Window object type has been removed.
	void	destroyAllWindows(void);


	/*!
	\brief
		Creates a set of windows (a Gui layout) from the information in the specified XML file.	

	\param filename
		String object holding the filename of the XML file to be processed.

	\param name_prefix
		String object holding the prefix that is to be used when creating the windows in the layout file, this
		function allows a layout to be loaded multiple times without having name clashes.  Note that if you use
		this facility, then all windows defined within the layout must have names assigned; you currently can not
		use this feature in combination with automatically generated window names.

    \param resourceGroup
        Resource group identifier to be passed to the resource provider when loading the layout file.

	\param callback
		PropertyCallback function to be called for each Property element loaded from the layout.  This is
		called prior to the property value being applied to the window enabling client code manipulation of
		properties.

	\param userdata
		Client code data pointer passed to the PropertyCallback function.

	\return
		Pointer to the root Window object defined in the layout.

	\exception FileIOException			thrown if something goes wrong while processing the file \a filename.
	\exception InvalidRequestException	thrown if \a filename appears to be invalid.
	*/
	Window*	loadWindowLayout(const String& filename, const String& name_prefix = "", const String& resourceGroup = "", PropertyCallback* callback = 0, void* userdata = 0);

    bool isDeadPoolEmpty(void) const;

    /*!
    \brief
        Permanently destroys any windows placed in the dead pool.

    \note
        It is probably not a good idea to call this from a Window based event handler
        if the specific window has been or is being destroyed.

    \return
        Nothing.
    */
    void cleanDeadPool(void);

    /*!
    \brief
        Writes a full XML window layout, starting at the given Window to the given OutStream.

    \param window
        Window object to become the root of the layout.

    \param out_stream
        OutStream (std::ostream based) object where data is to be sent.

    \param writeParent
        If the starting window has a parent window, specifies whether to write the parent name into
        the Parent attribute of the GUILayout XML element.

    \return
        Nothing.
    */
    void writeWindowLayoutToStream(const Window& window, OutStream& out_stream, bool writeParent = false) const;

    /*!
    \brief
        Writes a full XML window layout, starting at the given Window to the given OutStream.

    \param window
        String holding the name of the Window object to become the root of the layout.

    \param out_stream
        OutStream (std::ostream based) object where data is to be sent.

    \param writeParent
        If the starting window has a parent window, specifies whether to write the parent name into
        the Parent attribute of the GUILayout XML element.

    \return
        Nothing.
    */
    void writeWindowLayoutToStream(const String& window, OutStream& out_stream, bool writeParent = false) const;

    /*!
    \brief
        Save a full XML window layout, starting at the given Window, to a file
        with the given file name.

    \param window
        String holding the name of the Window object to become the root of the
        layout.

    \param filename
        The name of the file to which the XML will be written.  Note that this
        does not use any part of the ResourceProvider system, but rather will
        write directly to disk.  If this is not desirable, you should prefer the
        OutStream based writeWindowLayoutToStream functions.

    \param writeParent
        If the starting window has a parent window, specifies whether to write
        the parent name into the Parent attribute of the GUILayout XML element.
    */
    void saveWindowLayout(const String& window, const String& filename, const bool writeParent = false) const;

    /*!
    \brief
        Save a full XML window layout, starting at the given Window, to a file
        with the given file name.

    \param window
        Window object to become the root of the layout.

    \param filename
        The name of the file to which the XML will be written.  Note that this
        does not use any part of the ResourceProvider system, but rather will
        write directly to disk.  If this is not desirable, you should prefer the
        OutStream based writeWindowLayoutToStream functions.

    \param writeParent
        If the starting window has a parent window, specifies whether to write
        the parent name into the Parent attribute of the GUILayout XML element.
    */
    void saveWindowLayout(const Window& window, const String& filename, const bool writeParent = false) const;

    //exception UnknownObjectException thrown if \a window is not known in the system.
    //exception AlreadyExistsException thrown if a Window named \a new_name already exists.
    void renameWindow(const String& window, const String& new_name);
    //exception AlreadyExistsException thrown if a Window named \a new_name already exists.
    void renameWindow(Window* window, const String& new_name);
    static const String& getDefaultResourceGroup()
        { return d_defaultResourceGroup; }
    static void setDefaultResourceGroup(const String& resourceGroup)
        { d_defaultResourceGroup = resourceGroup; }

    void lock();
    void unlock();
    bool isLocked() const;

private:
    /*************************************************************************
        Implementation Methods
    *************************************************************************/
	//一个固定的字符串加上一个递增的数字
    String generateUniqueWindowName();

    //! function to set up RenderEffect on a window
    void initialiseRenderEffect(Window* wnd, const String& effect) const;

	/*************************************************************************
		Implementation Constants
	*************************************************************************/
	static const char	GUILayoutSchemaName[];			//!< Filename of the XML schema used for validating GUILayout files.


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef std::map<String, Window*, String::FastLessCompare>			WindowRegistry;				//!< Type used to implement registry of Window objects
    typedef std::vector<Window*>    WindowVector;   //!< Type to use for a collection of Window pointers.

	WindowRegistry			d_windowRegistry;			//!< The container that forms the Window registry
    WindowVector    d_deathrow;     //!< Collection of 'destroyed' windows.

    unsigned long   d_uid_counter;  //!< Counter used to generate unique window names.
    static String d_defaultResourceGroup;   //!< holds default resource group
    //! count of times WM is locked against new window creation.
    uint    d_lockCount;

public:
	/*************************************************************************
		Iterator stuff
	*************************************************************************/
	typedef	ConstBaseIterator<WindowRegistry>	WindowIterator;
	WindowIterator	getIterator(void) const;

    /*!
    \brief
    Outputs the names of ALL existing windows to log (DEBUG function).

    \param zone
    Helper string that can specify where the name dump was made (e.g. "BEFORE FRAME DELETION").

    \return
    Nothing.
    */
    void DEBUG_dumpWindowNames(String zone);    
};

}