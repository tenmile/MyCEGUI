#pragma once

#include "CEGUIString.h"

/*************************************************************************
    The following is basically taken from DynLib.h, which is part of
    the Ogre project (http://www.ogre3d.org/)
*************************************************************************/
#if defined(__WIN32__) || defined(_WIN32)
#   define DYNLIB_HANDLE hInstance
#   define DYNLIB_LOAD( a ) LoadLibrary( a )
#   define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#   define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

    struct HINSTANCE__;
    typedef struct HINSTANCE__* hInstance;

#elif defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY )
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )
#    define DYNLIB_ERROR( ) dlerror( )

#elif defined(__APPLE_CC__)
#    define DYNLIB_HANDLE CFBundleRef
#    define DYNLIB_LOAD( a ) mac_loadExeBundle( a )
#    define DYNLIB_GETSYM( a, b ) mac_getBundleSym( a, b )
#    define DYNLIB_UNLOAD( a ) mac_unloadExeBundle( a )
#    define DYNLIB_ERROR( ) mac_errorBundle()
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Class that wraps and gives access to a dynamically linked module
    (.dll, .so, etc...)
*/
class DynamicModule
{
public:
    DynamicModule(const String& name);
    ~DynamicModule();
    const String& getModuleName() const;
    //exception InvalidRequestException thrown if the symbol does not exist.
    void* getSymbolAddress(const String& symbol) const;

private:
    /*!
    \brief
        Return a String containing the last failure message from the platforms
        dynamic loading system.
    */
    String getFailureString() const;

    //! Holds the name of the loaded module.
    String d_moduleName;
    //! Handle for the loaded module
    DYNLIB_HANDLE d_handle;
};

}