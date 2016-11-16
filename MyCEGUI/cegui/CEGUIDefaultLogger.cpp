#include "CEGUIDefaultLogger.h"
#include <ctime>
#include <iomanip>

namespace CEGUI
{

    /*************************************************************************
        Constructor
    *************************************************************************/
    DefaultLogger::DefaultLogger(void) :
            d_caching(true)
    {
        // create log header
        logEvent("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+");
        logEvent("+                     Crazy Eddie's GUI System - Event log                    +");
        logEvent("+                          (http://www.cegui.org.uk/)                         +");
        logEvent("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
        char addr_buff[32];
        sprintf(addr_buff, "(%p)", static_cast<void*>(this));
        logEvent("CEGUI::Logger singleton created. " + String(addr_buff));
    }

    /*************************************************************************
        Destructor
    *************************************************************************/
    DefaultLogger::~DefaultLogger(void)
    {
        if (d_ostream.is_open())
        {
            char addr_buff[32];
            sprintf(addr_buff, "(%p)", static_cast<void*>(this));
            logEvent("CEGUI::Logger singleton destroyed. " + String(addr_buff));
            d_ostream.close();
        }
    }

    /*************************************************************************
        Logs an event
    *************************************************************************/
    void DefaultLogger::logEvent(const String& message, LoggingLevel level /* = Standard */)
    {
        using namespace std;

        time_t  et;
        time(&et);
        tm* etm = localtime(&et);

        if (etm)
        {
            // clear sting stream
            d_workstream.str("");

            // write date
            d_workstream << setfill('0') << setw(2) << etm->tm_mday << '/' <<
            setfill('0') << setw(2) << 1 + etm->tm_mon << '/' <<
            setw(4) << (1900 + etm->tm_year) << ' ';

            // write time
            d_workstream << setfill('0') << setw(2) << etm->tm_hour << ':' <<
            setfill('0') << setw(2) << etm->tm_min << ':' <<
            setfill('0') << setw(2) << etm->tm_sec << ' ';

            // write event type code
            switch(level)
            {
            case Errors:
                d_workstream << "(Error)\t";
                break;

            case Warnings:
                d_workstream << "(Warn)\t";
                break;

            case Standard:
                d_workstream << "(Std) \t";
                break;

            case Informative:
                d_workstream << "(Info) \t";
                break;

            case Insane:
                d_workstream << "(Insan)\t";
                break;

            default:
                d_workstream << "(Unkwn)\t";
                break;
            }

            d_workstream << message << endl;

            if (d_caching)
            {
                d_cache.push_back(std::make_pair(d_workstream.str(), level));
            }
            else if (d_level >= level)
            {
                // write message
                d_ostream << d_workstream.str();
                // ensure new event is written to the file, rather than just being buffered.
                d_ostream.flush();
            }
        }
    }

    void DefaultLogger::setLogFilename(const String& filename, bool append)
    {
        // close current log file (if any)
        if (d_ostream.is_open())
        {
            d_ostream.close();
        }

        d_ostream.open(filename.c_str(), std::ios_base::out | (append ? std::ios_base::app : std::ios_base::trunc));

        if (!d_ostream)
        {
            CEGUI_THROW("Logger::setLogFilename - Failed to open file.");
        }

        // initialise width for date & time alignment.
        d_ostream.width(2);

        // write out cached log strings.
        if (d_caching)
        {
            d_caching = false;

            std::vector<std::pair<String, LoggingLevel> >::iterator iter = d_cache.begin();

            while (iter != d_cache.end())
            {
                if (d_level >= (*iter).second)
                {
                    // write message
                    d_ostream << (*iter).first;
                    // ensure new event is written to the file, rather than just being buffered.
                    d_ostream.flush();
                }

                ++iter;
            }

            d_cache.clear();
        }
    }

}