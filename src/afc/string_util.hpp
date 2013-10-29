/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2013 Dźmitry Laŭčuk

libafc is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef AFC_STRING_UTIL_HPP_
#define AFC_STRING_UTIL_HPP_

#include <string>
#include "Exception.h"

namespace afc
{
	bool startsWith(const std::string &str, const std::string &substr) noexcept;
	bool endsWith(const std::string &str, const std::string &substr) noexcept;

	template<typename CharType = char>
	class Tokeniser
	{
		Tokeniser(const Tokeniser &) = delete;
		Tokeniser(Tokeniser &&) = delete;
		Tokeniser &operator=(const Tokeniser &) = delete;
		Tokeniser &operator=(Tokeniser &&) = delete;
	public:
		Tokeniser(const std::basic_string<CharType> &str, const CharType delimiter)
			: m_str(str), m_delimiter(delimiter), m_begin(0) {}
		Tokeniser(const std::basic_string<CharType> &&str, const CharType delimiter)
			: m_inputCopy(str), m_str(m_inputCopy), m_delimiter(delimiter), m_begin(0) {}

		bool hasNext() const throw() {return m_begin != std::basic_string<CharType>::npos;}
		std::basic_string<CharType> next();
	private:
		/* m_inputCopy is used only with temporary strings because they die early.
		   In other cases no copying of the input string is used. */
		const std::basic_string<CharType> m_inputCopy;
		const std::basic_string<CharType> &m_str;
		const CharType m_delimiter;
		size_t m_begin;
	};
}

template<typename CharType>
std::basic_string<CharType> afc::Tokeniser<CharType>::next()
{
	if (m_begin == std::basic_string<CharType>::npos) {
		throwException<IllegalStateException>("Tokeniser::next is called when it has not more tokens");
	}
	const size_t end = m_str.find(m_delimiter, m_begin);
	if (end == std::basic_string<CharType>::npos) {
		auto token = m_str.substr(m_begin);
		m_begin = std::basic_string<CharType>::npos;
		return token;
	} else {
		auto token = m_str.substr(m_begin, end - m_begin);
		m_begin = end + 1;
		return token;
	}
}

#endif /* AFC_STRING_UTIL_HPP_ */
