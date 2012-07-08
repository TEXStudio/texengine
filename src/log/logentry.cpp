#include <cstring>
#include "logentry.hpp"

namespace TEXEngine {
	namespace Util {

LogEntry::LogEntry() :
	p_type(LOGENTRY_MESSAGE)
{}

LogEntry::LogEntry(const LogEntry &rhs)
{}

LogEntry & LogEntry::operator=(const LogEntry &rhs)
{
	return *this;
}

	} /* Util */
} /* TEXEngine */