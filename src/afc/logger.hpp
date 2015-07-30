/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2015 Dźmitry Laŭčuk

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

#ifndef AFC_LOGGER_HPP_
#define AFC_LOGGER_HPP_

#include <cstdio>
#include <initializer_list>
#include <string>
#include <type_traits>

// POSIX API.
#include <stdio.h>

#include "ensure_ascii.hpp" // needed to ensure that '\n' != EOF.
#include "number.h"
#include "SimpleString.hpp"
#include "StringRef.hpp"

namespace afc
{
	namespace logger
	{
		class FileLock
		{
		public:
			FileLock(std::FILE * const file) noexcept : m_file(file) { flockfile(file); }
			~FileLock() { funlockfile(m_file); }
		private:
			std::FILE * const m_file;
		};

		inline bool logText(const char * const s, const std::size_t n, FILE * const dest) noexcept
		{
			return std::fwrite(s, sizeof(char), n, dest) == n;
		}

		template<typename T>
		inline typename std::enable_if<std::is_integral<T>::value, bool>::type logPrint(T value, FILE * const dest) noexcept
		{
			char buf[afc::maxPrintedSize<T, 10>()];
			char *end = afc::printNumber<10>(value, buf);
			return logText(buf, end - buf, dest);
		}

		inline bool logPrint(afc::ConstStringRef s, FILE * const dest) noexcept
		{
			return logText(s.value(), s.size(), dest);
		}

		inline bool logPrint(afc::SimpleString &s, FILE * const dest) noexcept
		{
			return logText(s.data(), s.size(), dest);
		}

		inline bool logPrint(const std::string &s, FILE * const dest) noexcept
		{
			return logText(s.data(), s.size(), dest);
		}

		inline bool logPrint(const char * const s, FILE * const dest) noexcept
		{
			return std::fputs(s, dest) >= 0;
		}

		inline bool logPrint(const std::pair<const char *, const char *> &s, FILE * const dest) noexcept
		{
			return logText(s.first, std::size_t(s.second - s.first), dest);
		}

		template<typename T>
		inline typename std::enable_if<std::is_floating_point<T>::value, bool>::type logPrint(T value, FILE * const dest) noexcept
		{
			// TODO improve performance.
			return logPrint(std::to_string(value), dest);
		}

		class Printer
		{
		public:
			/* There is no need in the virtual destructor since each Printer instances
			 * are allocated only on the stack.
			 */
			virtual bool operator()(std::FILE *dest) = 0;
		};

		template<typename T>
		class LogPrinter : public Printer
		{
			// The basic types are copied, non-basic are passed by reference.
			typedef typename std::conditional<std::is_pointer<T>::value || std::is_arithmetic<T>::value,
					const T, const T &>::type storage_type;
		public:
			LogPrinter(const T &value) noexcept : m_value(value) {}

			bool operator()(std::FILE *dest) { return logPrint(m_value, dest); }

			LogPrinter *address() noexcept { return this; }
		private:
			storage_type m_value;
		};

		template<typename T>
		inline LogPrinter<typename std::decay<const T>::type> logPrinter(const T &val) noexcept
		{
			return LogPrinter<typename std::decay<const T>::type>(val);
		}

		bool logInternal(const char *format, std::initializer_list<Printer *> params, FILE *dest);

		template<typename... Args>
		inline bool logToFile(std::FILE * const dest, const char *format, const Args &...args)
		{
			/* Passing polymorphic instances of Printer to logInternal.
			 *
			 * All the temporary objects live until logInternal returns so all pointers to
			 * local objects passed to logInternal are valid.
			 */
			return logInternal(format, {logPrinter(args).address()...}, dest);
		}

		template<typename T>
		inline bool logToFileMsg(const T &message, std::FILE * const dest) noexcept
		{ FileLock fileLock(dest);
			return logPrint(message, dest) && std::fputc('\n', dest) != EOF;
		}

		template<typename T>
		bool logDebugMsg(const T &message) noexcept;

		template<typename... Args>
		bool logDebug(const char *format, const Args &...args);

		#ifdef NDEBUG
			template<typename T>
			inline bool logDebugMsg(const T &message) noexcept { /* Nothing to do. */ return true; }

			template<typename... Args>
			inline bool logDebug(const char *format, const Args &...args) { /* Nothing to do. */ return true; }
		#else
			template<typename T>
			inline bool logDebugMsg(const T &message) noexcept
			{
				bool success = logToFileMsg(message, stdout);
				// stdout is flushed so that the message logged becomes visible immediately.
				success &= std::fflush(stdout) != EOF;
				return success;
			}

			template<typename... Args>
			inline bool logDebug(const char * const format, const Args &...args)
			{
				bool success = logToFile(stdout, format, args...);
				// stdout is flushed so that the message logged becomes visible immediately.
				success &= std::fflush(stdout);
				return success;
			}
		#endif

		template<typename T>
		inline bool logErrorMsg(const T &message) noexcept { return logToFileMsg(message, stderr); }

		template<typename... Args>
		inline bool logError(const char * const format, const Args &...args) { return logToFile(stderr, format, args...); }
	}
}

#endif /* AFC_LOGGER_HPP_ */
