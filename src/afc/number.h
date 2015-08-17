/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2015 Dźmitry Laŭčuk

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
#ifndef AFC_NUMBER_H_
#define AFC_NUMBER_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>

#include "builtin.hpp"
#include "math_utils.h"

namespace afc
{
	namespace number_limits
	{
		const unsigned MIN_BASE = 2;
		const unsigned MAX_BASE = 36;
	}

	namespace _impl
	{
		// Return value is negated for negative limits.
		template<typename T, int base, bool positive>
		constexpr T safeLimit() noexcept
		{
			return positive ?
					(std::numeric_limits<T>::max() - base + 1) / base :
					(std::is_signed<T>::value ? -((std::numeric_limits<T>::min() + base - 1) / base) : 0);
		}

		// Return value is negated for negative limits.
		template<typename T, int base, bool positive>
		constexpr T mulLimit() noexcept
		{
			return positive ?
					std::numeric_limits<T>::max() / base :
					(std::is_signed<T>::value ? -(std::numeric_limits<T>::min() / base) : 0);
		}

		static constexpr char digitChars[number_limits::MAX_BASE] =
			{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
			 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
			 'u', 'v', 'w', 'x', 'y', 'z'};
	}

	/* TODO check thoroughly if this table can provide better performance
	 * against bit manipulations on real load (cache miss effects).
	 */
	static const char octetToHexTable[256][2] = {
			{'0', '0'}, {'0', '1'}, {'0', '2'}, {'0', '3'}, {'0', '4'}, {'0', '5'}, {'0', '6'}, {'0', '7'},
			{'0', '8'}, {'0', '9'}, {'0', 'a'}, {'0', 'b'}, {'0', 'c'}, {'0', 'd'}, {'0', 'e'}, {'0', 'f'},
			{'1', '0'}, {'1', '1'}, {'1', '2'}, {'1', '3'}, {'1', '4'}, {'1', '5'}, {'1', '6'}, {'1', '7'},
			{'1', '8'}, {'1', '9'}, {'1', 'a'}, {'1', 'b'}, {'1', 'c'}, {'1', 'd'}, {'1', 'e'}, {'1', 'f'},
			{'2', '0'}, {'2', '1'}, {'2', '2'}, {'2', '3'}, {'2', '4'}, {'2', '5'}, {'2', '6'}, {'2', '7'},
			{'2', '8'}, {'2', '9'}, {'2', 'a'}, {'2', 'b'}, {'2', 'c'}, {'2', 'd'}, {'2', 'e'}, {'2', 'f'},
			{'3', '0'}, {'3', '1'}, {'3', '2'}, {'3', '3'}, {'3', '4'}, {'3', '5'}, {'3', '6'}, {'3', '7'},
			{'3', '8'}, {'3', '9'}, {'3', 'a'}, {'3', 'b'}, {'3', 'c'}, {'3', 'd'}, {'3', 'e'}, {'3', 'f'},
			{'4', '0'}, {'4', '1'}, {'4', '2'}, {'4', '3'}, {'4', '4'}, {'4', '5'}, {'4', '6'}, {'4', '7'},
			{'4', '8'}, {'4', '9'}, {'4', 'a'}, {'4', 'b'}, {'4', 'c'}, {'4', 'd'}, {'4', 'e'}, {'4', 'f'},
			{'5', '0'}, {'5', '1'}, {'5', '2'}, {'5', '3'}, {'5', '4'}, {'5', '5'}, {'5', '6'}, {'5', '7'},
			{'5', '8'}, {'5', '9'}, {'5', 'a'}, {'5', 'b'}, {'5', 'c'}, {'5', 'd'}, {'5', 'e'}, {'5', 'f'},
			{'6', '0'}, {'6', '1'}, {'6', '2'}, {'6', '3'}, {'6', '4'}, {'6', '5'}, {'6', '6'}, {'6', '7'},
			{'6', '8'}, {'6', '9'}, {'6', 'a'}, {'6', 'b'}, {'6', 'c'}, {'6', 'd'}, {'6', 'e'}, {'6', 'f'},
			{'7', '0'}, {'7', '1'}, {'7', '2'}, {'7', '3'}, {'7', '4'}, {'7', '5'}, {'7', '6'}, {'7', '7'},
			{'7', '8'}, {'7', '9'}, {'7', 'a'}, {'7', 'b'}, {'7', 'c'}, {'7', 'd'}, {'7', 'e'}, {'7', 'f'},
			{'8', '0'}, {'8', '1'}, {'8', '2'}, {'8', '3'}, {'8', '4'}, {'8', '5'}, {'8', '6'}, {'8', '7'},
			{'8', '8'}, {'8', '9'}, {'8', 'a'}, {'8', 'b'}, {'8', 'c'}, {'8', 'd'}, {'8', 'e'}, {'8', 'f'},
			{'9', '0'}, {'9', '1'}, {'9', '2'}, {'9', '3'}, {'9', '4'}, {'9', '5'}, {'9', '6'}, {'9', '7'},
			{'9', '8'}, {'9', '9'}, {'9', 'a'}, {'9', 'b'}, {'9', 'c'}, {'9', 'd'}, {'9', 'e'}, {'9', 'f'},
			{'a', '0'}, {'a', '1'}, {'a', '2'}, {'a', '3'}, {'a', '4'}, {'a', '5'}, {'a', '6'}, {'a', '7'},
			{'a', '8'}, {'a', '9'}, {'a', 'a'}, {'a', 'b'}, {'a', 'c'}, {'a', 'd'}, {'a', 'e'}, {'a', 'f'},
			{'b', '0'}, {'b', '1'}, {'b', '2'}, {'b', '3'}, {'b', '4'}, {'b', '5'}, {'b', '6'}, {'b', '7'},
			{'b', '8'}, {'b', '9'}, {'b', 'a'}, {'b', 'b'}, {'b', 'c'}, {'b', 'd'}, {'b', 'e'}, {'b', 'f'},
			{'c', '0'}, {'c', '1'}, {'c', '2'}, {'c', '3'}, {'c', '4'}, {'c', '5'}, {'c', '6'}, {'c', '7'},
			{'c', '8'}, {'c', '9'}, {'c', 'a'}, {'c', 'b'}, {'c', 'c'}, {'c', 'd'}, {'c', 'e'}, {'c', 'f'},
			{'d', '0'}, {'d', '1'}, {'d', '2'}, {'d', '3'}, {'d', '4'}, {'d', '5'}, {'d', '6'}, {'d', '7'},
			{'d', '8'}, {'d', '9'}, {'d', 'a'}, {'d', 'b'}, {'d', 'c'}, {'d', 'd'}, {'d', 'e'}, {'d', 'f'},
			{'e', '0'}, {'e', '1'}, {'e', '2'}, {'e', '3'}, {'e', '4'}, {'e', '5'}, {'e', '6'}, {'e', '7'},
			{'e', '8'}, {'e', '9'}, {'e', 'a'}, {'e', 'b'}, {'e', 'c'}, {'e', 'd'}, {'e', 'e'}, {'e', 'f'},
			{'f', '0'}, {'f', '1'}, {'f', '2'}, {'f', '3'}, {'f', '4'}, {'f', '5'}, {'f', '6'}, {'f', '7'},
			{'f', '8'}, {'f', '9'}, {'f', 'a'}, {'f', 'b'}, {'f', 'c'}, {'f', 'd'}, {'f', 'e'}, {'f', 'f'}
	};

	static const unsigned char asciiToDigit[256] = {
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
			25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
			25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
	};

	enum class ParseMode
	{
		scan,
		all
	};

	// TODO think how to declare noexcept
	template<typename OutputIterator>
	OutputIterator octetToHex(const unsigned char o, const OutputIterator dest)
	{
		assert(o >= 0 && o <= 0xff);
		OutputIterator end = dest;
		const char *p = octetToHexTable[o];
		*end++ = *p;
		*end++ = *(p + 1);
		return end;
	}

	template<unsigned char base, typename T>
	constexpr std::size_t printedSize(T val) noexcept
	{
		static_assert(std::is_integral<T>::value, "T must be an integral type.");
		static_assert(std::is_unsigned<T>::value, "T must be an unsigned type.");

		return val < base ? 1 : 1 + printedSize<base, T>(val / base);
	}

	template<typename T, unsigned char base>
	constexpr std::size_t maxPrintedSize() noexcept
	{
		static_assert(std::is_integral<T>::value, "T must be an integral type.");

		typedef typename std::make_unsigned<T>::type UnsignedT;
		return std::is_signed<T>::value ?
				// One character for sign.
				printedSize<base>(std::numeric_limits<UnsignedT>::max()) + 1 :
				printedSize<base>(std::numeric_limits<UnsignedT>::max());
	}

	template<unsigned char base, typename T>
	constexpr std::size_t digitCount(const T val)
	{
		static_assert(std::is_integral<T>::value, "An integral type is expected.");
		static_assert(base >= 2, "Base must be greater than or equal to two.");
		return (val < 0 && val > -static_cast<T>(base)) || (val >= 0 && val < base) ?
				1 : 1 + digitCount<base, T>(val / base);
	}

	template<unsigned char base, typename T>
	constexpr std::size_t maxDigitCount()
	{
		static_assert(std::is_integral<T>::value, "T must be an integral type.");

		/* The min signed value contains at least as many digits as the max signed value
		 * for all signed type representations allowed in C++11. However, calculating
		 * it for both min and max just in case something changes. It is free of charge
		 * anyway.
		 */
		return std::is_signed<T>::value ?
				afc::math::max(digitCount<base>(std::numeric_limits<T>::min()), digitCount<base>(std::numeric_limits<T>::min())) :
				digitCount<base>(std::numeric_limits<T>::max());
	}

	// TODO use std::enable_if
	template<unsigned char base, typename T>
	constexpr char digitToChar(const T digit) noexcept
	{
		static_assert(std::is_integral<T>::value, "T must be an integral type.");
		static_assert(base >= afc::number_limits::MIN_BASE && base <= afc::number_limits::MAX_BASE, "Unsupported base.");

		return base <= 10 ? '0' + digit : _impl::digitChars[digit];
	};

	// A hex digit must be passed in.
	template<typename T>
	constexpr char hexToChar(const T digit) noexcept { return digitToChar<16>(digit); }

	// TODO think of defining conditional noexcept.
	template<unsigned char base, typename T, typename OutputIterator>
	OutputIterator printNumber(const T value, OutputIterator dest);

	// TODO test this for all types and bases
	// TODO optimise performance (especially for binary bases)
	/* TODO modify iterator-based afc::printNumber so that it is possible to avoid copying digits
	 * to the intermediate buffer (in the reverse order).
	 */
	template<unsigned char base, typename T>
	inline void printNumber(const T value, std::string &out)
	{
		char buf[maxPrintedSize<T, base>()];

		char * const begin = &buf[0];
		char * const end = printNumber<base>(value, begin);
		out.append(begin, end);
	}

	// TODO think of defining conditional noexcept.
	template<unsigned char base, afc::ParseMode parseMode = ParseMode::all,
			typename T, typename Iterator, typename ErrorHandler>
	Iterator parseNumber(Iterator begin, Iterator end, T &result, ErrorHandler errorHandler);

	template<typename T, typename Iterator>
	inline Iterator printTwoDigits(const T value, Iterator dest)
	{
		assert(value >= 0 && value < 100);

		const std::uint_fast8_t high = value / 10;
		const std::uint_fast8_t low = value - high * 10;
		*dest++ = afc::digitToChar<10>(high);
		*dest++ = afc::digitToChar<10>(low);
		return dest;
	}

	template<typename T, typename Iterator>
	inline Iterator printFourDigits(const T value, Iterator dest)
	{
		assert(value >= 0 && value <= 9999);

		const std::uint_fast16_t hundreds = value / 100;
		dest = afc::printTwoDigits(hundreds, dest);
		dest = afc::printTwoDigits(value - hundreds * 100, dest);
		return dest;
	}
}

template<unsigned char base, typename T, typename Iterator>
Iterator afc::printNumber(const T value, register Iterator dest)
{
	static_assert(std::is_integral<T>::value, "Integral types are supported only.");
	static_assert(base >= afc::number_limits::MIN_BASE && base <= afc::number_limits::MAX_BASE, "Unsupported base.");

	typedef typename std::make_unsigned<T>::type UnsignedT;

	// TODO use direct order and bulk append.

	// The buffer that contains digits in the reverse order. The greatest digit is not stored here.
	char digits[maxDigitCount<base, T>() - 1];
	std::size_t i = 0;

	/** If value is equal to the min signed value then the negation of it is either
	 *  max signed value (for ones' complement and sign/magnitude signed representations) or
	 *  min signed value itself (for the two's complement representation). In the latter case
	 *  the min signed value is casted to the correspondent unsigned value correctly as if
	 *  it were properly negated.
	 */
	UnsignedT val;
	if (value >= 0) {
		val = value;
	} else {
		val = static_cast<UnsignedT>(-value);
		*dest++ = '-';
	}

	while (val >= base) {
		const UnsignedT nextVal = val / base;
		digits[i++] = digitToChar<base>(val - nextVal * base);
		val = nextVal;
	}
	assert(i <= sizeof(digits));

	// The highest digit.
	*dest++ = digitToChar<base>(val);
	while (i != 0) {
		*dest++ = digits[--i];
	}
	return dest;
}

// TODO optimise performance
template<unsigned char base, afc::ParseMode parseMode, typename T, typename Iterator, typename ErrorHandler>
Iterator afc::parseNumber(Iterator begin, Iterator end, T &result, ErrorHandler errorHandler)
{
	static_assert(std::is_integral<T>::value, "Integral types are supported only.");
	static_assert(base >= afc::number_limits::MIN_BASE && base <= afc::number_limits::MAX_BASE, "Unsupported base.");
	static_assert(parseMode == ParseMode::all || parseMode == ParseMode::scan, "Unsupported parsing mode.");

	if (begin == end) {
		errorHandler(begin);
		return end;
	}

	Iterator p = begin;
	char c;
	T sign;
	T safeLimit;
	if (std::is_signed<T>::value) {
		c = *p;
		if (c == u8"-"[0]) {
			if (++p == end) {
				errorHandler(end);
				return p;
			}
			sign = -1;
			safeLimit = _impl::safeLimit<T, base, false>();
		} else {
			sign = 1;
			safeLimit = _impl::safeLimit<T, base, true>();
		}
	} else {
		safeLimit = _impl::safeLimit<T, base, true>();
	}

	result = 0;
	do {
		const unsigned char c = static_cast<unsigned char>(*p);
		if (c > 0xff) {
			errorHandler(p);
			return p;
		}
		const T digit = asciiToDigit[c];
		if (unlikely(digit >= base)) {
			if (parseMode == ParseMode::all || p == begin) {
				errorHandler(p);
				return p;
			} else if (parseMode == ParseMode::scan) {
				break;
			}
		}

		if (likely(result <= safeLimit)) {
			result *= base;
			result += digit;
		} else {
			T mulLimit;
			T addLimit;
			if (std::is_signed<T>::value) {
				if (sign < 0) {
					mulLimit = _impl::mulLimit<T, base, false>();
					addLimit = std::numeric_limits<T>::min();
				} else {
					mulLimit = _impl::mulLimit<T, base, true>();
					addLimit = -std::numeric_limits<T>::max();
				}
			} else {
				mulLimit = _impl::mulLimit<T, base, true>();
				addLimit = std::numeric_limits<T>::max();
			}

			if (likely(result <= mulLimit)) {
				result *= base;
				if (std::is_signed<T>::value) {
					if (likely(addLimit + result + digit <= 0)) {
						result += digit;
						continue;
					}
				} else {
					if (likely(result <= addLimit - digit)) {
						result += digit;
						continue;
					}
				}
			}
			errorHandler(p);
			return p;
		}
	} while (++p != end);

	if (std::is_signed<T>::value) {
		result *= sign; // Works even for min value for all sign encoding schemes.
	}

	return p;
}

#endif /*AFC_NUMBER_H_*/
