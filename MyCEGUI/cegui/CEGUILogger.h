#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include "CEGUISingleton.h"


namespace CEGUI
{

/*!
\brief
	Enumeration of logging levels
*/
enum LoggingLevel
{
	Errors,			//!< Only actual error conditions will be logged.
    Warnings,       //!< Warnings will be logged as well.
	Standard,		//!< Basic events will be logged (default level).
	Informative,	//!< Useful tracing (object creations etc) information will be logged.
	Insane			//!< Mostly everything gets logged (use for heavy tracing only, log WILL be big).
};

/*!
\brief
	Abstract class that defines the interface of a logger object for the GUI system.
    The default implementation of this interface is the DefaultLogger class; if you
    want to perform special logging, derive your own class from Logger and initialize
    a object of that type before you create the CEGUI::System singleton.
*/
class Logger : public Singleton <Logger>
{
public:
	/*!
	\brief
		Constructor for Logger object.
	*/
	Logger(void);

	/*!
	\brief Destructor for Logger object.
	*/
	virtual ~Logger(void);


	/*!
	\brief
		Set the level of logging information that will get out to the log file

	\param level
		One of the LoggingLevel enumerated values that specified the level of logging information required.

	\return
		Nothing
	*/
	void	setLoggingLevel(LoggingLevel level)		{d_level = level;}


	/*!
	\brief
		return the current logging level setting

	\return
		One of the LoggingLevel enumerated values specifying the current level of logging
	*/
	LoggingLevel	getLoggingLevel(void) const		{return d_level;}


	/*!
	\brief
		Add an event to the log.

	\param message
		String object containing the message to be added to the event log.

	\param level
		LoggingLevel for this message.  If \a level is greater than the current set logging level, the message is not logged.

	\return
		Nothing
	*/
	virtual void logEvent(const String& message, LoggingLevel level = Standard) = 0;

    /*!
    \brief
        Set the name of the log file where all subsequent log entries should be written.
        The interpretation of file name may differ depending on the concrete logger
        implementation.

    \note
        When this is called, and the log file is created, any cached log entries are
        flushed to the log file.

    \param filename
        Name of the file to put log messages.

    \param append
        - true if events should be added to the end of the current file.
        - false if the current contents of the file should be discarded.
     */
    virtual void setLogFilename(const String& filename, bool append = false) = 0;

protected:
	LoggingLevel	d_level;		//!< Holds current logging level

private:
	/*************************************************************************
		Copy constructor and assignment usage is denied.
	*************************************************************************/
	Logger(const Logger&) : Singleton <Logger>() {}
	Logger& operator=(const Logger&) {return *this;}

};

/*************************************************************************
	This macro is used for 'Insane' level logging so that those items are
	excluded from non-debug builds
*************************************************************************/
#if defined(DEBUG) || defined (_DEBUG)
#	define CEGUI_LOGINSANE( message ) CEGUI::Logger::getSingleton().logEvent((message), CEGUI::Insane);
#else
#	define CEGUI_LOGINSANE( message )
#endif

}