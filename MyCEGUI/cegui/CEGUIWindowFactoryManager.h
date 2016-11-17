#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUILogger.h"
#include "CEGUIIteratorBase.h"
#include "CEGUIWindowFactory.h"
#include <map>
#include <vector>

namespace CEGUI
{
/*! 
\brief
	Class that manages WindowFactory objects

\todo
    I think we could clean up the mapping stuff a bit. Possibly make it more generic now
    with the window renderers and all.
*/
class WindowFactoryManager : public Singleton<WindowFactoryManager>
{
public:
    struct FalagardWindowMapping
    {
        String  d_windowType;
        String  d_lookName;
        String  d_baseType;
        String  d_rendererType;
        String  d_effectName;
    };

    /*!
    \brief
        Class used to track active alias targets for Window factory types.
    */
	class AliasTargetStack
	{
	public:
		AliasTargetStack(void) {}
		~AliasTargetStack(void) {}
		//生效的是最后一个
		const String&	getActiveTarget(void) const;
		uint	getStackedTargetCount(void) const;
	private:
		friend class WindowFactoryManager;
		typedef std::vector<String>	TargetTypeStack;		//!< Type used to implement stack of target type names.

		TargetTypeStack	d_targetStack;		//!< Container holding the target types.
	};


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WindowFactoryManager(void);
	~WindowFactoryManager(void)
	{
		Logger::getSingleton().logEvent("CEGUI::WindowFactoryManager singleton destroyed");
	}


	/*************************************************************************
		Public Interface
	*************************************************************************/
	//exception NullObjectException		\a factory was null.
	//exception AlreadyExistsException	\a factory provided a Window type name which is in use by another registered WindowFactory.
	void	addFactory(WindowFactory* factory);
	//static
	//会更新d_ownedFactories
    template <typename T>
    static void addFactory();
	void	removeFactory(const String& name);
	void	removeFactory(WindowFactory* factory);
    void removeAllFactories(void);
	//exception UnknownObjectException	No WindowFactory object for Window objects of type \a type was found.
	WindowFactory*	getFactory(const String& type) const;
    bool	isFactoryPresent(const String& name) const;
	//exception UnknownObjectException	thrown if \a targetType is not known within the system.
	void	addWindowTypeAlias(const String& aliasName, const String& targetType);
	void	removeWindowTypeAlias(const String& aliasName, const String& targetType);

    /*!
    \brief
        Add a mapping for a falagard based window.

        This function creates maps a target window type and target 'look' name onto a registered window type, thus allowing
        the ususal window creation interface to be used to create windows that require extra information to full initialise
        themselves.
    \note
        These mappings support 'late binding' to the target window type, as such the type indicated by \a targetType need not
        exist in the system until attempting to create a Window using the type.
    \par
        Also note that creating a mapping for an existing type will replace any previous mapping for that same type.

    \param newType
        The type name that will be used to create windows using the target type and look.

    \param targetType
        The base window type.

    \param lookName
        The name of the 'look' that will be used by windows of this type.

    \param renderer
        The type of window renderer to assign for windows of this type.

    \param effectName
        The identifier of the RenderEffect to attempt to set up for windows of this type.

    \return
        Nothing.
    */
    void addFalagardWindowMapping(const String& newType,
                                  const String& targetType,
                                  const String& lookName,
                                  const String& renderer,
                                  const String& effectName = String(""));

    void removeFalagardWindowMapping(const String& type);
    bool isFalagardMappedType(const String& type) const;

    /*!
    \brief
        Return the name of the LookN'Feel assigned to the specified window mapping.

    \param type
        Name of a window type.  The window type referenced should be a falagard mapped type.

    \return
        String object holding the name of the look mapped for the requested type.

    \exception InvalidRequestException thrown if \a type is not a falagard mapping type (or maybe the type didn't exist).
    */
    const String& getMappedLookForType(const String& type) const;

    /*!
    \brief
        Return the name of the WindowRenderer assigned to the specified window mapping.

    \param type
        Name of a window type.  The window type referenced should be a falagard mapped type.

    \return
        String object holding the name of the window renderer mapped for the requested type.

    \exception InvalidRequestException thrown if \a type is not a falagard mapping type (or maybe the type didn't exist).
    */
    const String& getMappedRendererForType(const String& type) const;

    /*!
    \brief
        Use the alias system, where required, to 'de-reference' the specified
        type to an actual window type that can be created directly (that being
        either a concrete window type, or a falagard mapped type).

    \note
        Even though implied by the above description, this method does not
        check that a factory for the final type exists; we simply say that the
        returned type is not an alias for some other type.

    \param type
        String describing the type to be de-referenced.

    \return
        String object holding a type for a window that can be created directly;
        that is, a type that does not describe an alias to some other type.
    */
    String getDereferencedAliasType(const String& type) const;

    /*!
    \brief
        Return the FalagardWindowMapping for the specified window mapping \a type.

    \param type
        Name of a window type.  The window type referenced should be a falagard mapped type.

    \return
        FalagardWindowMapping object describing the falagard mapping.

    \exception InvalidRequestException thrown if \a type is not a falagard mapping type (or maybe the type didn't exist).
    */
    const FalagardWindowMapping& getFalagardMappingForType(const String& type) const;

private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef	std::map<String, WindowFactory*, String::FastLessCompare>	WindowFactoryRegistry;		//!< Type used to implement registry of WindowFactory objects
	typedef std::map<String, AliasTargetStack, String::FastLessCompare>	TypeAliasRegistry;		//!< Type used to implement registry of window type aliases.
    typedef std::map<String, FalagardWindowMapping, String::FastLessCompare> FalagardMapRegistry;    //!< Type used to implement registry of falagard window mappings.
    //! Type used for list of WindowFacory objects that we created ourselves
    typedef std::vector<WindowFactory*> OwnedWindowFactoryList;

	WindowFactoryRegistry	d_factoryRegistry;			//!< The container that forms the WindowFactory registry
	TypeAliasRegistry		d_aliasRegistry;			//!< The container that forms the window type alias registry.
    FalagardMapRegistry     d_falagardRegistry;         //!< Container that hold all the falagard window mappings.
    //! Container that tracks WindowFactory objects we created ourselves.
    static OwnedWindowFactoryList  d_ownedFactories;

public:
	/*************************************************************************
		Iterator stuff
	*************************************************************************/
	typedef	ConstBaseIterator<WindowFactoryRegistry>	WindowFactoryIterator;
	typedef ConstBaseIterator<TypeAliasRegistry>		TypeAliasIterator;
    typedef ConstBaseIterator<FalagardMapRegistry>      FalagardMappingIterator;

	/*!
	\brief
		Return a WindowFactoryManager::WindowFactoryIterator object to iterate over the available WindowFactory types.
	*/
	WindowFactoryIterator	getIterator(void) const;


	/*!
	\brief
		Return a WindowFactoryManager::TypeAliasIterator object to iterate over the defined aliases for window types.
	*/
	TypeAliasIterator	getAliasIterator(void) const;


    /*!
    \brief
        Return a WindowFactoryManager::FalagardMappingIterator object to iterate over the defined falagard window mappings.
    */
    FalagardMappingIterator getFalagardMappingIterator() const;
};

//----------------------------------------------------------------------------//
template <typename T>
void WindowFactoryManager::addFactory()
{
    // create the factory object
    WindowFactory* factory = new T;

    // only do the actual add now if our singleton has already been created
    if (WindowFactoryManager::getSingletonPtr())
    {
        Logger::getSingleton().logEvent("Created WindowFactory for '" +
                                        factory->getTypeName() +
                                        "' windows.");
        // add the factory we just created
        CEGUI_TRY
        {
            WindowFactoryManager::getSingleton().addFactory(factory);
        }
        CEGUI_CATCH (Exception&)
        {
            Logger::getSingleton().logEvent("Deleted WindowFactory for '" +
                                            factory->getTypeName() +
                                            "' windows.");
            // delete the factory object
            delete factory;
            CEGUI_RETHROW;
        }
    }

    d_ownedFactories.push_back(factory);
}

}