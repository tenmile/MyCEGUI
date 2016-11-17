#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIWindow.h"

/*!
\brief
    Declares a window factory class.

\param T
    The window class name.

\note
    The class that will be generated is is named &lt;classname&gt;Factory.
    A statement like this:
        CEGUI_DECLARE_WINDOW_FACTORY(MyWidget);
    Would generate a factory class named MyWidgetFactory.

    The factory is automatically instantiated and for the example it would
    be available as:
        WindowFactory* wf = &(getMyWidgetFactory());
    or
        WindowFactory* wf = &CEGUI_WINDOW_FACTORY(MyWidget);
*/
#define CEGUI_DECLARE_WINDOW_FACTORY( T )\
    class T ## Factory : public WindowFactory\
    {\
    public:\
        T ## Factory() : WindowFactory( T::WidgetTypeName ) {}\
        Window* createWindow(const String& name)\
        {\
            return new T (d_type, name);\
        }\
        void destroyWindow(Window* window)\
        {\
            delete window;\
        }\
    };\
    T ## Factory& get ## T ## Factory();

/*!
\brief
    Generates code for the constructor for the instance of the window factory
    generated from the class name \a T
*/
//static对象，窗口唯一，另一种实现单例的方式？
#define CEGUI_DEFINE_WINDOW_FACTORY( T )\
    T ## Factory& get ## T ## Factory()\
    {\
        static T ## Factory s_factory;\
        return s_factory;\
    }

/*!
\brief
    Helper macro that return the real factory class name from a given class
    name \a T
*/
#define CEGUI_WINDOW_FACTORY( T ) (get ## T ## Factory())

namespace CEGUI
{
/*!
\brief
    Abstract class that defines the required interface for all WindowFactory
    objects.

    A WindowFactory is used to create and destroy windows of a specific type.
    For every type of Window object wihin the system (widgets, dialogs, movable
    windows etc) there must be an associated WindowFactory registered with the
    WindowFactoryManager so that the system knows how to create and destroy
    those types of Window base object.

\note
    The use if of the CEGUI_DECLARE_WINDOW_FACTORY, CEGUI_DEFINE_WINDOW_FACTORY
    and CEGUI_WINDOW_FACTORY macros is deprecated in favour of the
    template class TplWindowFactory and templatised
    WindowFactoryManager::addFactory function, whereby you no longer need to
    directly create any supporting structure for your new window type, and can
    simply do:
    \code
    CEGUI::WindowFactoryManager::addFactory<TplWindowFactory<MyWidget> >();
    \endcode
*/
class WindowFactory
{
public:
    //name唯一
    virtual Window* createWindow(const String& name) = 0;
    virtual void destroyWindow(Window* window) = 0;
    const String& getTypeName() const
    { return d_type; }

    //! Destructor.
    virtual ~WindowFactory()
    {}

protected:
    //! Constructor
    WindowFactory(const String& type) :
        d_type(type)
    {}

protected:
    //! String holding the type of object created by this factory.
    String  d_type;
};

}