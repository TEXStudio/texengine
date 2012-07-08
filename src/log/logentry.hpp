#ifndef TEXENGINE_LOGENTRY_HPP_INCLUDED
#define TEXENGINE_LOGENTRY_HPP_INCLUDED

#include <string>

namespace TEXEngine {

	namespace Core { 
		class LogManager; // Forward declaration
	}

	using Core::LogManager;

	namespace Util {

enum LOGENTRY_TYPE
{
	LOGENTRY_MESSAGE,
	LOGENTRY_WARNING,
	LOGENTRY_ERROR
};

/*
	To do:
		- Add timestamps (by using TEXEngine::Port::Timer)
*/

class LogEntry
{
	friend class LogManager;
	protected:
		LogEntry();

		LOGENTRY_TYPE p_type;
		std::string p_msg;

	private:
		LogEntry(const LogEntry &rhs);
		LogEntry & operator=(const LogEntry &rhs);
};

	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_LOGENTRY_HPP_INCLUDED */