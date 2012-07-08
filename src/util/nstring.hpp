#ifndef TEXENGINE_NSTRING_HPP_INCLUDED
#define TEXENGINE_NSTRING_HPP_INCLUDED

#include <string>

namespace TEXEngine {
	namespace Util {
		namespace NString {

// Trims '"' at start and end of string. (eg.  "Hello" -> Hello )
void unquote(std::string &s);

// Strips the string of all occurences of the given character.
void strip(std::string &s, char c);

// Trims ' ', '\t' at the start of the given string.
void trim_left(std::string &s);

// Trims ' ', '\n', '\t', '\r' at the end of the given string.
void trim_right(std::string &s);

// Trims ' ', '\n', '\t', '\r' at the start and the end of the given string.
void trim(std::string &s);

// Splits the given array into two string at first occurance of given char.
void split(std::string &in, std::string &left, std::string &right, char c);

// Extracts the first balanced substring included between two pX chars.
void extract(std::string &s, std::string &t, const char p1, const char p2);

// Replace a matched substring p with r.
void replace_first_of(std::string &s, const std::string &p, const std::string &r);

// Converts string to lower case.
void to_lower_case(std::string &s);

// Converts string to upper case.
void to_upper_case(std::string &s);

// Converts string to boolean.
bool to_bool(const std::string &s);

// Converts string to integer.
int to_int(const std::string &s);

// Converts string to float.
float to_float(const std::string &s);

// Converts string to double.
double to_double(const std::string &s);

// Extracts the base name and path from a string.
void path_comp(const std::string &s, std::string &base, std::string &file);

		} /* namespace NString */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_NSTRING_HPP_INCLUDED */

