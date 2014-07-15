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
#ifndef AFC_MD5_HPP_
#define AFC_MD5_HPP_
#include <ctime>
#include <string>
#include <cstddef>
#include <openssl/md5.h>
#include "ensure_ascii.hpp"
#include "utils.h"

namespace afc
{
	template<typename OutputIterator>
	OutputIterator md5String(const unsigned char * const data, std::size_t n, const OutputIterator dest)
	{
		unsigned char hash[MD5_DIGEST_LENGTH];
		MD5(data, n, hash);

		OutputIterator p = dest;
		for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i) {
			const unsigned char b = hash[i] & 0xff;
			// 0xff is applied just in case non-octet bytes are used.
			*p++ = toHex(b >> 4);
			*p++ = toHex(b & 0xf);
		}
		return p;
	}

	/*
	 * Calculates the MD5 hash of a given binary data and writes it in the lower-case hex format
	 * as an ASCII sequence to the destination string.
	 *
	 * @param data the binary data whose MD5 hash is to be calculated.
	 * @param n the size of the binary data.
	 * @param dest the destination string. It is expected to be ASCII-compatible.
	 */
	inline void md5String(const unsigned char * const data, std::size_t n, std::string &dest)
	{
		// Contains encoded hash value.
		char hashString[2 * MD5_DIGEST_LENGTH];

		md5String(data, n, &hashString[0]);

		dest.append(hashString, 2 * MD5_DIGEST_LENGTH);
	}
}

#endif /* AFCMD5_HPP_ */
