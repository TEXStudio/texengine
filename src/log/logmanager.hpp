#ifndef TEXENGINE_LOGMANAGER_HPP_INCLUDED
#define TEXENGINE_LOGMANAGER_HPP_INCLUDED

#include <vector>
#include "logentry.hpp"
#include "../message/Object.hpp"

// Disable: "<type> needs to have dll-interface to be used by clients"
// This warning refers to STL member variables which are private and
// therefore can be safely ignored.
#pragma warning (push)
#pragma warning (disable : 4251)

using TEXEngine::Util::LOGENTRY_TYPE;
using TEXEngine::Util::LOGENTRY_MESSAGE;
using TEXEngine::Util::LOGENTRY_WARNING;
using TEXEngine::Util::LOGENTRY_ERROR;
using TEXEngine::Util::LogEntry;

namespace TEXEngine {
	namespace Core {

class LogManager : public Object
{
	public:
		// Returns a handle to the log manager.
		static LogManager &get();

		void on_event( const unsigned long event_code , const Message& parameters );
		void setup();
		
		// Logging functions.
		void log(LOGENTRY_TYPE type, const char * msg, ...);
		void log_message(const char *msg, ...);
		void log_error(const char *msg, ...);
		void log_warning(const char *msg, ...);

		// Set/Get the echo flag.
		void echo(bool state);
		bool echo() const;

		// Releases all the log entries.
		void clear();

		// Dumps all the log entries to the given file.
		// RETURN CODES:
		//	0: No errors.
		//	1: I/O error.
		int dump(const char* fpath);

		std::string get_log_entry(const unsigned int idx) const;

	protected:
		LogManager();
		~LogManager();

	private:
		// Private utility for general purpose logging.
		void pulog(LOGENTRY_TYPE type, const char *msg, va_list args);

		// Post process logging.
		void post();

		static LogManager m_log_manager;
		std::vector<LogEntry*> m_log;

		bool m_flag_echo;
};

	} /* namespace Core */
} /* namespace TEXEngine */

#pragma warning (pop)

#endif /* TEXENGINE_LOGMANAGER_HPP_INCLUDED */