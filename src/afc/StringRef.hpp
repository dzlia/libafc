/* libafc - utils to facilitate C++ development.
Copyright (C) 2014 Dźmitry Laŭčuk

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
#ifndef AFC_CONSTSTRINGREF_HPP_
#define AFC_CONSTSTRINGREF_HPP_

#include <cstddef>

namespace afc
{
	// Allows for efficient processing of string literals by resolving their size at compile time.
	class ConstStringRef
	{
	public:
		ConstStringRef(const ConstStringRef &) = default;
		ConstStringRef(ConstStringRef &&) = default;
		ConstStringRef &operator=(const ConstStringRef &) = default;
		ConstStringRef &operator=(ConstStringRef &&) = default;
		~ConstStringRef() = default;

		template <std::size_t n>
		ConstStringRef(const char (&str)[n]) : m_str(str), m_size(n - 1) {}

		explicit operator const char *() const { return m_str; }

		const char *value() { return m_str; }
		const size_t size() const { return m_size; }

		char operator[](size_t i) const { return m_str[i]; };

		const char *begin() const { return &m_str[0]; };
		const char *end() const { return &m_str[m_size]; };
	private:
		const char *m_str;
		const size_t m_size;
	};
}

#endif /* AFC_CONSTSTRINGREF_HPP_ */
