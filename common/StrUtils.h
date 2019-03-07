#ifndef __STRUTILS_H__
#define __STRUTILS_H__

#include <string>
#include <cctype>

#ifdef _MSC_VER
#	pragma once
#endif

namespace Str {
	inline std::string toLower(const std::string& in)
	{
		std::string out;
		for (char ch : in)
		{
			out += std::tolower(ch);
		}

		return out;
	}

	inline std::string toUpper(const std::string& in)
	{
		std::string out;
		for (char ch : in)
		{
			out += std::toupper(ch);
		}

		return out;
	}
}

#endif
