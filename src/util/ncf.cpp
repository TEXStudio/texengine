#include <fstream>
#include <cstdio>
#include <iostream>
#include <cstdio>
#include "nstring.hpp"
#include "ncf.hpp"

using namespace TEXEngine::Util::NString;

namespace TEXEngine {
	namespace Util {

NCF1::NCF1()
	: m_p_level(0)
{}

NCF1 &NCF1::operator =(const NCF1 &)
{
	// This constructor is private and is never used internally.
	// Therefore:
	// 1. I do not check about self assignement.
	// 2. I do not check about shallow copying ( no copying will ever be performed ).
	return *this;
}

NCF1::~NCF1()
{
	destroy();
}

void NCF1::set_source(const char *file)
{
	// Purge the previous configuration.
	purge();
	m_p_filepath = file;
}

const char* NCF1::get_source() const
{
	return m_p_filepath.c_str();
}

int NCF1::parse()
{
	{
		#ifdef NCF1_DEBUG
			printf("CF1 Parser: First pass, checking integrity\n");
		#endif
	
		int c;
		std::ifstream a(m_p_filepath.c_str());

		do {
			c = a.get();
		}while(c != EOF && c <= 127);

		a.close();

		if(c != EOF)
		{
			#ifdef NCF1_DEBUG
				printf("CF1 Parser: not an ascii file\n");
			#endif
			return 1;
		}
	}

	/* Stack of groups for parsing */
    std::list< NCF1* > group_stack;

	group_stack.push_front(this);

    /* Process file */
    std::fstream in(m_p_filepath.c_str(), std::ios::in);

	if (!in.good())
	{
		/* File I/O error */
		#ifdef NCF1_DEBUG
			printf("CF1 Parser: %s -> I/O error\n", m_p_filepath.c_str());
		#endif
		return 1;
	}

	int linep = 0;
	int sectionp = 0;

	std::string input;
	while (getline(in, input))
	{
		linep++;

	    std::string line, comment;
	    /* Strip comments */
	    split(input, line, comment, '#');
	    /* Trim spaces and unused characters */
		comment.empty();
		trim(line);

		if (!line.length())
		{
			/* empty line - do nothing */
		}
		else if (line.length() > 2 && (line.find('=') != std::string::npos))
		{
			/* get the identifier and its value */
			std::string name;
			std::string value;
			split(line, name, value, '=');

			/* Trim the name and its value from unused characters and spaces */
			trim(name);
			trim(value);

			/* If this is a section start */
			if (value == "{")
			{
				sectionp++;

				std::map<std::string, NCF1* >::iterator it = group_stack.front()->m_p_groups.find(name);

				/* Check whether the group name already exists and create a new node, if needed */
				if (it == group_stack.front()->m_p_groups.end())
				{
					NCF1 *new_group = new NCF1();
					new_group->m_p_level = group_stack.front()->m_p_level + 1;
					group_stack.front()->m_p_groups[name] = new_group;
					new_group->m_p_name = name;

					#ifdef NCF1_DEBUG
						printf("CF1 Parser: [+node: %p]\n", (void *)new_group);
					#endif
				}

				/* Make it the active node */
				group_stack.push_front(group_stack.front()->m_p_groups[name]);
			}
			/* If this is a regular assignment */
			else
			{
				for (std::list< NCF1* >::reverse_iterator it = group_stack.rbegin(); it != group_stack.rend(); ++it)
					(*it)->expand_symbol(m_p_symbols, value);

				group_stack.front()->m_p_symbols[name] = value;
			}
		}
		/* If this is a section end */
        else if( (line.length() == 1) && (line.find('}') != std::string::npos) )
        {
			sectionp--;

			/* Check for unbalanced sections to prevent segfaults */
			if (sectionp < 0)
			{
				std::cerr << "Syntax error: Unbalanced sections at line: " << linep << std::endl;
				return 1;
			}

			group_stack.pop_front();
        }
		else if((line.length() > 1) && (line.find('%') != std::string::npos))
		{
			/* get the identifier and its value */
			std::string com;
			std::string value;
			split(line, com, value, ' ');
			/* Trim the com and its value from unused characters and spaces */
			trim(com);
			trim(value);


			if (!com.compare("%include"))
			{
				std::string base, file;
				/* Extract base path and file name of the active script */
				path_comp(m_p_filepath, base, file);

				/* Append the inclusion value to the base */
				base.append(value);

				#ifdef NCF1_DEBUG
				printf("CF1 Parser: From: %s ", m_p_filepath.c_str());
					printf("Including %s\n", base.c_str());
				#endif /* NCF1_DEBUG */
				group_stack.front()->m_p_filepath = base;
				for (std::map<std::string, std::string>::iterator it = m_p_symbols.begin(); it != m_p_symbols.end(); it++)
					group_stack.front()->set((*it).first.c_str(), (*it).second.c_str());
				group_stack.front()->parse();				
			}
			else
			{
				std::cerr << "Syntax error: Invalid modifier [ " << com.c_str() << " ] at line: " << linep << "." << std::endl;
			}
		}
		else
		{
			std::cerr << "Syntax error: Invalid entry at line: " << linep << "." << std::endl;
			return 1;
		}

	}

	/* Check for unbalanced sections */
	if(sectionp)
	{
		std::cerr << "Syntax error: Final unbalanced sections [ " << linep << " ].";
	}

	in.close();

	return 0;
}

int NCF1::dump(const char *file, int create)
{
	/* Process file */
    std::ios_base::openmode fmode = std::ios_base::out;

    if (!create)
		fmode = fmode | std::ios_base::app;

    std::fstream out(file, fmode);

	if (!out.good())
	{
		/* File I/O error */
		#ifdef NCF1_DEBUG
			printf("CF1 Parser: %s -> I/O error\n", file);
		#endif
		return 1;
	}

	if (create)
		out << "# CF1 dump\n";

	/* Prepare identation string */
	std::string ident;
	for (unsigned int i = 0; i < m_p_level; i++)
		ident.append("\t");

	for (std::map<std::string, std::string>::iterator it = m_p_symbols.begin(); it != m_p_symbols.end(); ++it)
	{
		out << ident.c_str() << it->first.c_str() << " = " << it->second.c_str() << "\n";
		out.flush();
	}

	for (std::map<std::string, NCF1* >::reverse_iterator it = m_p_groups.rbegin(); it != m_p_groups.rend(); ++it)
	{
		out << ident.c_str() << it->first.c_str() << " = {" << "\n";
		out.flush();
		out.close();

		it->second->dump(file, 0);

		fmode = fmode | std::ios_base::app;
		out.open(file, fmode);

		if (!out.good())
		{
			/* File I/O error */
			#ifdef NCF1_DEBUG
				printf("CF1 Parser: %s -> I/O error\n", file);
			#endif /* NCF1_DEBUG */
			return 1;
		}

		out << ident.c_str() << "}" << "\n";
	}

	out.flush();
	out.close();
	return 0;
}

void NCF1::expand_symbol(std::map<std::string, std::string> &symbols, std::string &s)
{
	if (s.empty() || (s.length() < 3) || (s.find_first_of('<') == s.npos) || (s.find_first_of('<') == s.find_last_of('>')))
        return; /* Nothing to expand, so return */

	std::string source = s;

	do
	{
		std::string search;

		/* Extract reference */
		extract(source,search,'<','>');

		/* If there is no valid pattern, return */
		if (search.length() < 3)
			return;

		/* Purify string */
		std::string psearch = search.substr(1,search.length() - 2);

		/* Try to match */
		for (std::map<std::string, std::string>::iterator it = symbols.begin(); it != symbols.end(); ++it)
		{
			#ifdef NCF1_DEBUG
				printf("CF1 Parser: %s == %s ?\n", search.c_str(), it->first.c_str());
			#endif /* NCF1_DEBUG */

			size_t pos = psearch.compare(it->first);
			if (!pos)
			{
				replace_first_of(s, search, it->second);
				#ifdef NCF1_DEBUG
					printf("CF1 Parser: Matched! %s\n", s.c_str());
				#endif /* NCF1_DEBUG */
			}
		}
	} while (1);
}

void NCF1::purge()
{
	destroy();
	m_p_symbols.clear();
}

void NCF1::destroy()
{
	for (std::map<std::string, NCF1* >::reverse_iterator it = m_p_groups.rbegin(); it != m_p_groups.rend(); it++)
	{
		delete it->second;
		#ifdef NCF1_DEBUG
			printf("CF1 Parser: [-node: %p]\n", (void *)it->second);
		#endif /* NCF1_DEBUG */
	}
	m_p_groups.clear();
}

const char* NCF1::get(const char *name) const
{
	std::map<std::string, std::string>::iterator it = m_p_symbols.find(name);
	return (it == m_p_symbols.end()) ? "" : it->second.c_str();
}

const char* NCF1::get(unsigned int index) const
{
	unsigned int c = count_properties();
	
	if (!c)
		return NULL;

	std::map<std::string, std::string>::iterator it = m_p_symbols.begin();
	
	if (index >= c)
		index = c-1;	
	
	while (index > 0)
	{
		index--;
		it++;
	}

	return it->second.c_str();
}

const char* NCF1::get_name(unsigned int index) const
{
	unsigned int c = count_properties();
	
	if (!c)
		return NULL;

	std::map<std::string, std::string>::iterator it = m_p_symbols.begin();
	
	if (index >= c)
		index = c-1;	
	
	while (index > 0)
	{
		index--;
		it++;
	}

	return it->first.c_str();
}

void NCF1::set(const char *name, const char *value)
{
	m_p_symbols[name] = value;
}

NCF1* NCF1::group(const char *name) const
{
    std::map<std::string, NCF1* >::const_iterator it = m_p_groups.find(name);
    /* If the requested node key doesn't exist then we create it */
    if (it == m_p_groups.end())
    {
    	m_p_groups[name] = new NCF1();
    	m_p_groups[name]->m_p_level = m_p_level + 1;
		#ifdef NCF1_DEBUG
			printf("CF1 Parser: [!node: %p]\n", (void *)m_p_groups[name]);
		#endif /* NCF1_DEBUG */
    }

    return m_p_groups[name];
}

NCF1* NCF1::group(unsigned int index) const
{
	unsigned int c = count_groups();
	
	if (!c)
		return NULL;

	std::map<std::string, NCF1* >::const_iterator it = m_p_groups.begin();
	
	if (index >= c)
		index = c-1;	
	
	while (index > 0)
	{
		index--;
		it++;
	}

	return it->second;
}

unsigned int NCF1::count_groups() const
{
	return m_p_groups.size();
}

unsigned int NCF1::count_properties() const
{
	return m_p_symbols.size();
}

void NCF1::list_groups(std::string &l, char delim)
{
	l.clear();

	unsigned int c = 0;

	for (std::map<std::string, NCF1* >::iterator it = m_p_groups.begin(); it != m_p_groups.end(); it++)
	{
		l.append((*it).first);

		if(++c < m_p_groups.size())
		{
			l.append(1, delim);
		}
	}
}

void NCF1::list_properties(std::string &l, char delim)
{
	l.clear();
	
	unsigned int c = 0;

	for (std::map<std::string, std::string >::iterator it = m_p_symbols.begin(); it != m_p_symbols.end(); it++)
	{
		l.append((*it).first);

		if(++c < m_p_groups.size())
		{
			l.append(1, delim);
		}
	}
}

const char* NCF1::name() const
{
	return m_p_name.c_str();
}

unsigned int NCF1::query_property(const char* name) const
{
	return m_p_symbols.find(name) != m_p_symbols.end() ? 1 : 0;
}

unsigned int NCF1::query_group(const char* name) const
{
	return m_p_groups.find(name) != m_p_groups.end() ? 1 : 0;
}

NCF1::NCF1(const NCF1&)
	: m_p_level(0)
{}

	} /* namespace Util */
} /* namespace TEXEngine */