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
#ifndef AFC_COMPILE_TIME_MATH_H_
#define AFC_COMPILE_TIME_MATH_H_

#include <climits>
#include <type_traits>
#include <limits>

namespace afc
{
	template<typename T>
	constexpr unsigned bitCount(const T val)
	{
		static_assert(std::is_integral<T>::value, "An integral type is expected.");
		return val == 0 || val == 1 ? 1 : bitCount(val >> 1) + 1;
	}

	template<typename T>
	constexpr unsigned onesCount(const T val)
	{
		static_assert(std::is_integral<T>::value, "An integral type is expected.");
		return val == 0 ? 0 : (val & 1) + onesCount(val >> 1);
	}

	template<unsigned val> struct LeadZeroCount
	{
	private:
		static const unsigned rightmostOneValue = 1 << (std::numeric_limits<unsigned>::digits - 1);
	public:
		enum {result = (val&LeadZeroCount::rightmostOneValue) == 0 ? LeadZeroCount<(val>>1)>::result - 1 : 0};
	};
	template<> struct LeadZeroCount<0>
	{
		enum {result = std::numeric_limits<unsigned>::digits};
	};

	template<unsigned val> struct Log2
	{
		enum
		{
			floor = std::numeric_limits<unsigned>::digits - 1 - LeadZeroCount<val>::result,
			ceil = std::numeric_limits<unsigned>::digits - LeadZeroCount<val-1>::result,
		};
	};
	template<> struct Log2<0>;

	template<int x, int y> struct IntMin
	{
		enum {result = x < y ? x : y};
	};

	template<int x, int y> struct IntMax
	{
		enum {result = x < y ? y : x};
	};
}

#endif /*AFC_COMPILE_TIME_MATH_H_*/
