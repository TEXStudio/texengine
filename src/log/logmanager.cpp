#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdarg.h>

#include "logmanager.hpp"
#include "../message/TEXEngineInterface.hpp"

namespace TEXEngine {
	namespace Core {

LogManager LogManager::m_log_manager;

LogManager::LogManager() :
	Object(9) , 
	m_flag_echo(true)
{}

LogManager::~LogManager()
{
	clear();
}

void LogManager::clear()
{
	std::vector<LogEntry *>::iterator it;

	for (it = m_log.begin(); it != m_log.end(); it++) {
		delete (*it);
	}

	m_log.clear();
}

int LogManager::dump(const char* fpath)
{
	std::ofstream file;
	file.open(fpath);

	if (!file.is_open())
		return 1;

	std::vector<LogEntry *>::iterator it;

	for (it = m_log.begin(); it != m_log.end(); it++) {
		switch ((*it)->p_type) {
			case LOGENTRY_MESSAGE:
				break;
			case LOGENTRY_WARNING:
				file << "Warning: ";
				break;
			case LOGENTRY_ERROR:
				file << "Error: ";
				break;
		}

		file << (*it)->p_msg << std::endl;
		file.flush();
	}

	file.close();

	return 0;
}

std::string LogManager::get_log_entry(const unsigned int idx) const{

	if ( idx   < m_log.size() )
		return m_log[m_log.size()-1-idx]->p_msg;
	
	return std::string("");
}

LogManager &LogManager::get()
{
	return m_log_manager;
}

void LogManager::echo(bool state)
{
	m_flag_echo = state;
}

bool LogManager::echo() const
{
	return m_flag_echo;
}

void LogManager::pulog(LOGENTRY_TYPE type, const char *msg, va_list args)
{

	// Store the length of the passed in string.
	unsigned int msg_size = strlen(msg);

	// We do not want to process an empty string.
	if(msg_size == 0)
		return;

	// This is the delimeter that is used to specify the format flags
	// you can use what you want.
	static char delim = '%';

    std::stringstream str;

	// The va_arg internal casting is inherently not safe here.
	for(unsigned int x = 0; x < msg_size; x++) {
		if(msg[x] == delim)	{
			// If there is a next character, and it is not a delim, 
			if(x + 1 < msg_size && msg[x + 1] != delim) {
				// String
				if (msg[x + 1] == 's') {
					// We have a c-string, so get it and save it
					const char* temp = va_arg(args, const char*);
					str << temp;
					x++;
				}

				else if (msg[x + 1] == 'c') {
					char temp = va_arg(args, char);
					str << temp;
					x++;
				}
				
				// Integer
				else if (msg[x + 1] == 'i') {
					int temp = va_arg(args, int);
					str << temp;
					x++;
				}

				// Float
				else if (msg[x + 1] == 'f') {
					double temp = va_arg(args, double);
					str << temp;
					x++;
				}
			} 
			else if(x + 1 < msg_size) {
				// Save the delim that "delimdelim" was used for
				str << delim;

				// We know what comes after the delim, so do not process it
				x++;
			}
		}
		// ("delimdelim" means add a single delim)
		else {
			str << msg[x];
		}
	}

	LogEntry *entry = new LogEntry();

	entry->p_type = type;
	entry->p_msg = str.str();

	m_log.push_back(entry);

	Message parameters(3,0,1);
	std::string	text(entry->p_msg);
	parameters.add_integral_data(0,get_id());
	parameters.add_integral_data(2,(MESSAGE_INTEGRAL_DATA_TYPE)type);
	parameters.add_pointer(0,&text);
	trigger_event(LOG_MANAGER_MESSAGE_LOGGED+type,parameters);
	trigger_event(LOG_MANAGER_ENTRY_LOGGED,parameters);

	post();
}

void LogManager::log(LOGENTRY_TYPE type, const char * msg, ...)
{
	va_list args;
	va_start(args, msg);
	pulog(type, msg, args);
	va_end(args);
}

void LogManager::log_message(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	pulog(LOGENTRY_MESSAGE, msg, args);
	va_end(args);
}

void LogManager::log_error(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	pulog(LOGENTRY_ERROR, msg, args);
	va_end(args);
}

void LogManager::log_warning(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	pulog(LOGENTRY_WARNING, msg, args);
	va_end(args);
}


void LogManager::post()
{
	std::vector<LogEntry *>::reverse_iterator it = m_log.rbegin();
	
	if(m_flag_echo) {
		switch ((*it)->p_type) {
			case LOGENTRY_MESSAGE:
				break;
			case LOGENTRY_WARNING:
				std::cout << "Warning: ";
				break;
			case LOGENTRY_ERROR:
				std::cout << "Error: ";
				break;
		}

		std::cout << (*it)->p_msg << std::endl;
	}
}

void LogManager::on_event( const unsigned long event_code , const Message& parameters )
{
	if ( event_code == EXIT )
	{
		std::string*	filename = static_cast<std::string*>(parameters.get_pointer(0));

		if ( filename )
			dump(filename->c_str());
	}
}

void LogManager::setup()
{
	register_event(EXIT);
}

	} /* namespace Core */
} /* namespace TEXEngine */

