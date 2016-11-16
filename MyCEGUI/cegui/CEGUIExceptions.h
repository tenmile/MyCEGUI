#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include <exception>


namespace CEGUI
{
//! Root exception class used within the GUI system.
class  Exception : public std::exception
{
public:
    //! Virtual destructor.
    virtual ~Exception(void) throw();
    const String& getMessage(void) const
        { return d_message; }
    const String& getName() const
        { return d_name; }
    const String& getFileName(void) const
        { return d_filename; }
    int getLine(void) const
        { return d_line; }

    // override from std::exception.
    const char* what() const throw();

protected:
	//protect，防止被用户实例化
    Exception(const String& message = "",
              const String& name = "CEGUI::Exception",
              const String& filename = "",
              int line = 0);

    //! Holds the reason for the exception being thrown.
    String d_message;
    //! Holds the name of the file where the exception occurred.
    String d_filename;
    //! Holds the class name of the exception (e.g. CEGUI::ObjectInUseException)
    String d_name;
    //! Holds the line number where the exception occurred.
    int d_line;
    //! Holds the default error message.
    String d_what;
};

//----------------------------------------------------------------------------//

//! Exception class used when none of the other classes are applicable
class GenericException : public Exception
{
public:
    GenericException(const String& message,
                     const String& file = "unknown", int line = 0) :
        Exception(message, "CEGUI::GenericException", file, line)
    {}
};

#define GenericException(message)  \
    GenericException(message, __FILE__, __LINE__)

//----------------------------------------------------------------------------//

//! Exception class used when a request was made for an unknown object
class UnknownObjectException : public Exception
{
public:
    UnknownObjectException(const String& message,
                           const String& file = "unknown", int line = 0) :
        Exception(message, "CEGUI::UnknownObjectException", file, line)
    {}
};

#define UnknownObjectException(message)  \
    UnknownObjectException(message, __FILE__, __LINE__)

//----------------------------------------------------------------------------//

//! Exception class used when some impossible request was made of the system.
class InvalidRequestException : public Exception
{
public:
    InvalidRequestException(const String& message,
                            const String& file = "unknown", int line = 0) :
        Exception(message, "CEGUI::InvalidRequestException", file, line)
    {}
};

#define InvalidRequestException(message)  \
    InvalidRequestException(message, __FILE__, __LINE__)

//----------------------------------------------------------------------------//

//! Exception class used when a file handling problem occurs
class FileIOException : public Exception
{
public:
    FileIOException(const String& message,
                    const String& file = "unknown", int line = 0) :
        Exception(message, "CEGUI::FileIOException", file, line)
    {}
};

#define FileIOException(message)  \
    FileIOException(message, __FILE__, __LINE__)

//----------------------------------------------------------------------------//

//! Exception class used for problems in the rendering subsystem classes.
class RendererException : public Exception
{
public:
    RendererException(const String& message,
                      const String& file = "unknown", int line = 0) :
        Exception(message, "CEGUI::RendererException", file, line)
    {}
};

#define RendererException(message)  \
    RendererException(message, __FILE__, __LINE__)

//----------------------------------------------------------------------------//

/*!
\brief
    Exception class used when an attempt is made create a named object of a
    particular type when an object of the same type already exists with the same
    name.
*/
class AlreadyExistsException : public Exception
{
public:
    AlreadyExistsException(const String& message,
                           const String& file = "unknown", int line = 0) :
        Exception(message, "CEGUI::AlreadyExistsException", file, line)
    {}
};

#define AlreadyExistsException(message)  \
    AlreadyExistsException(message, __FILE__, __LINE__)

//----------------------------------------------------------------------------//

//! Exception class used when a memory handling error is detected
class MemoryException : public Exception
{
public:
    MemoryException(const String& message,
                    const String& file = "unknown", int line = 0) :
        Exception(message, "CEGUI::MemoryException", file, line)
    {}
};

#define MemoryException(message)  \
    MemoryException(message, __FILE__, __LINE__)

//----------------------------------------------------------------------------//

//! Exception class used when some required object or parameter is null
class NullObjectException : public Exception
{
public:
    NullObjectException(const String& message,
                        const String& file = "unknown", int line = 0) :
        Exception(message, "CEGUI::NullObjectException", file, line)
    {}
};

#define NullObjectException(message)  \
    NullObjectException(message, __FILE__, __LINE__)

//----------------------------------------------------------------------------//

/*!
\brief
    Exception class used when some attempt to delete, remove, or otherwise
    invalidate some object that is still in use occurs.
*/
class ObjectInUseException : public Exception
{
public:
    ObjectInUseException(const String& message,
                         const String& file = "unknown", int line = 0) :
        Exception(message, "CEGUI::ObjectInUseException", file, line)
    {}
};

#define ObjectInUseException(message)  \
    ObjectInUseException(message, __FILE__, __LINE__)

//----------------------------------------------------------------------------//

//! Exception class used for issues in scripting subsystem.
class ScriptException : public Exception
{
public:
    ScriptException(const String& message,
                    const String& file = "unknown", int line = 0) :
        Exception(message, "CEGUI::ScriptException", file, line)
    {}
};

#define ScriptException(message)  \
    ScriptException(message, __FILE__, __LINE__)


//----------------------------------------------------------------------------//

}