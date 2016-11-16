#include "CEGUILogger.h"
namespace CEGUI
{

/*!
\brief
	Default implementation for the Logger class.
    If you want to redirect CEGUI logs to some place other than a text file,
    implement your own Logger implementation and create a object of the
    Logger type before creating the CEGUI::System singleton.
*/
class DefaultLogger : public Logger
{
public:
	/*!
	\brief
		Constructor for DefaultLogger object.
	*/
	DefaultLogger(void);

	/*!
	\brief Destructor for DefaultLogger object.
	*/
	virtual ~DefaultLogger(void);

	
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
	virtual void logEvent(const String& message, LoggingLevel level = Standard);

    /*!
    \brief
        Set the name of the log file where all subsequent log entries should be written.

    \note
        When this is called, and the log file is created, any cached log entries are
        flushed to the log file.

    \param filename
        Name of the file to put log messages.

    \param append
        - true if events should be added to the end of the current file.
        - false if the current contents of the file should be discarded.
     */
    virtual void setLogFilename(const String& filename, bool append = false);

protected:
    /*************************************************************************
		Implementation Data
	*************************************************************************/
	std::ofstream	d_ostream;		//!< Stream used to implement the logger
    std::vector<std::pair<String, LoggingLevel> > d_cache;    //!< Used to cache log entries before log file is created.
    std::ostringstream d_workstream;//!< Used to build log entry strings. 
    bool d_caching;                 //!< true while log entries are beign cached (prior to logfile creation)
};

}