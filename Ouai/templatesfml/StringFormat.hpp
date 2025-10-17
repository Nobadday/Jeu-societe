#ifndef STRINGFORMAT_H
#define STRINGFORMAT_H

#include "Common.hpp"

class StringFormat
{
public:
	// Equivalent de printf qui renvoie une std::string
	// Usage : auto s = StringFormat::format("x=%d y=%.2f", 10, 3.14);
	static std::string Format(const char* _fmt, ...);

	// Version contrôlée par va_list
	static std::string VFormat(const char* _fmt, va_list _args);
};

#endif // STRINGFORMAT_HPP