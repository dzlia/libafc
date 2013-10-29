/* libafc - utils to facilitate C++ development.
Copyright (C) 2013 Dźmitry Laŭčuk

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
#ifndef AFC_BASE64_HPP_
#define AFC_BASE64_HPP_

#include <string>

namespace afc
{
	std::string encodeBase64(const std::string &str);
}

#endif /* AFC_BASE64_HPP_ */
