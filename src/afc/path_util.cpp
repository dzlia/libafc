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
#include "path_util.hpp"
#include "platform.h"

using std::string;

#ifdef AFC_LINUX
	#include <limits.h>
	#include <unistd.h>
#elif defined AFC_WIN32
	#include <windows.h>
#elif defined AFC_UNIX
	#include <limits.h>
	#define _PSTAT64
	#include <sys/pstat.h>
	#include <sys/types.h>
	#include <unistd.h>
#else
	#error "unsupported system"
#endif

namespace afc
{
#ifdef AFC_LINUX
	String getExecPath()
	{
		char result[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		return String(result, (count > 0) ? count : 0);
	}
#elif defined AFC_WIN32
	String getExecPath()
	{
		char result[MAX_PATH];
		return String(result, GetModuleFileName(0, result, MAX_PATH));
	}
#elif defined AFC_UNIX
	String getExecPath()
	{
		char result[PATH_MAX];
		struct pst_status ps;

		if (pstat_getproc(&ps, sizeof(ps), 0, getpid()) < 0) {
			return String();
		}
		if (pstat_getpathname(result, PATH_MAX, &ps.pst_fid_text) < 0) {
			return String();
		}
		return String(result);
	}
#else
	#error "unsupported system"
#endif
}
