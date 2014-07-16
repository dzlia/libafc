/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2014 Dźmitry Laŭčuk

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
#ifndef AFCDATEUTIL_HPP_
#define AFCDATEUTIL_HPP_
#include <ctime>
#include <string>

/* getCurrentUTCTimeSeconds() relies upon posix-compatible format of std::time_t.
 * Each POSIX implementation must have unistd.h available.
 */
#include <unistd.h>
#include <time.h>

namespace afc
{
	class Timestamp
	{
	public:
		explicit Timestamp(const long millis) : m_millis(millis) {}

		long millis() { return m_millis; }
	private:
		long m_millis;
	};

	struct DateTime
	{
		DateTime() noexcept : gmtOffset(0), year(0), month(0), day(0), hour(0), minute(0), second(0), millisecond(0),
				isDst(-1) {}

		DateTime(const std::tm &dateTime) noexcept { *this = dateTime; }

		DateTime &operator=(const std::time_t timestamp) noexcept
		{
			std::tm dateTime;

			/* Initialises the system time zone data. According to POSIX.1-2004, localtime() is required
			 * to behave as though tzset(3) was called, while localtime_r() does not have this requirement.
			 */
			::tzset();
			::localtime_r(&timestamp, &dateTime);

			return *this = dateTime;
		}

		DateTime &operator=(const std::tm &dateTime) noexcept
		{
			gmtOffset = dateTime.tm_gmtoff; // Note: tm_gmtoff is not a part of the standard C++11.
			year = dateTime.tm_year;
			month = dateTime.tm_mon + 1;
			day = dateTime.tm_mday;
			hour = dateTime.tm_hour;
			minute = dateTime.tm_min;
			second = dateTime.tm_sec;
			millisecond = 0;
			isDst = dateTime.tm_isdst;
			return *this;
		}

		explicit operator std::tm() const noexcept
		{
			std::tm result;
			result.tm_gmtoff = gmtOffset; // Note: tm_gmtoff is not a part of the standard C++11.
			result.tm_year = year;
			result.tm_mon = month - 1;
			result.tm_mday = day;
			result.tm_hour = hour;
			result.tm_min = minute;
			result.tm_sec = second;
			result.tm_isdst = isDst;
			return result;
		}

		Timestamp timestamp() const noexcept
		{
			std::tm t = operator std::tm();
			/* This implementation works only for POSIX-compatible systems that store time in
			 * std::time_t as the number of seconds since epoch.
			 */
			return Timestamp(::mktime(&t));
		}

		// In seconds.
		long gmtOffset;

		long year;
		unsigned month;
		unsigned day;
		unsigned hour;
		unsigned minute;
		unsigned second;
		unsigned millisecond;
		int isDst;
	};

	// a utf-8 string is expected
	bool parseISODateTime(const std::string &str, std::time_t &dest);

	// a utf-8 string is expected
	bool parseISODateTime(const std::string &str, DateTime &dest);

	inline Timestamp now()
	{
		/* This implementation works only for POSIX-compatible systems that store time in
		 * std::time_t as the number of seconds since epoch.
		 */
		return Timestamp(::time(nullptr) * 1000);
	}
}

#endif /* AFCDATEUTIL_HPP_ */

