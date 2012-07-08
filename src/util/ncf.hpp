#ifndef TEXENGINE_NCF_HPP_INCLUDED
#define TEXENGINE_NCF_HPP_INCLUDED

#include <map>
#include <string>
#include <list>

/*
	Parsing rules
	-------------

	a. Each line is treated as separate rule entry.
	b. Trailing and leading whitespaces in entries are stripped at parsing stage.
	c. Parsed config entries are stored in a symbol map.
	d. Config files can be structured (to arbitrary depth).

	Entry types
	-----------

	include		:   %include <path_to_file>

	Comments	:	Strings beginning with '#' are comments and will be ignored.

	Assignments	:	Entries with name-value assignments follow the form:
						<name> = <value>

					Bottom-most assignment within a group, preveil.

	Sub sections:	Sub sections follow the form:
						<name> = {
							<content>
						}

					Again, content will be multilined, following the one command per line rule.
					'{' must be in the same line as the group name, following the pattern of assignments.
					'}' must be alone in an empty line.

					example of invalid formats
					--------------------------
					a.	<name> = { <content>
						}
					b.	<name> = {
						<content> }
					c.	<name> = { <content> }

	References	:	Values may reuse already defined symbols as variables which get expanded during the parsing process.
					The patterns for expansion are searched from the current sub group upwards, and follow the notation:
						<<Symbolic name>>

					Top-most references in group hierarchy preveil.
*/

// Uncomment the following line to activate runtime debug features
//#define NCF1_DEBUG

namespace TEXEngine {
	namespace Util {

class NCF1
{
	public:
		NCF1();
		~NCF1();

		/* Get source file path */
		void set_source(const char *file);					// Set the source file
		const char* get_source() const;						// Get the source file

		int parse();										// Parse the source file
		void purge();										// Purge the subtree
		int dump(const char *file, int create = 1);			// Dump to file
        
		const char* get(const char *name) const;			// Get property value by nam
		const char* get(unsigned int index) const;			// Get property value by index
		const char* get_name(unsigned int index) const;		// Get propery name by index
        void set(const char *name, const char *value);		// Set property value
		unsigned int count_properties() const;				// Count the properties
		void list_properties(std::string &l, char delim = ',');
															// Prepare a list of the properties
		unsigned int query_property(const char* name) const;
															// Return 1 if the property exists and 0 if not
		NCF1* group(const char *name) const;				// Get a pointer to a labeled sub group
		NCF1* group(unsigned int index) const;				// Get a pointer to an indexed sub group
		unsigned int count_groups() const;					// Count the subgroups
		void list_groups(std::string &l, char delim = ',');	// Prepare a list of the groups
		unsigned int query_group(const char *name) const;	// Return 1 if the group exists and 0 if not
		
		const char* name() const;							// Return the node's name

    private:
		// Dissallow copy construction.
		// Note: This will create a shallow copy and therefore it is NEVER used internally.
		// The reason is that the group tree is recursive and dynamic.
		NCF1(const NCF1&);
		NCF1 &operator =(const NCF1 &);

		void destroy();

		// Expand symbols.
		void expand_symbol(std::map<std::string, std::string> &symbols, std::string &s);

		std::string m_p_name;										// Node name
		mutable std::map<std::string, NCF1*> m_p_groups;			// Subnodes map
		mutable std::map<std::string, std::string> m_p_symbols;		// Symbol map
		std::string m_p_filepath;									// cfg filepath
		unsigned int m_p_level;										// Node level
};

	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_NCF_HPP_INCLUDED */