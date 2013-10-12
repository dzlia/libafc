#include "base64.hpp"
#include <cassert>
#include <limits>

using namespace std;

static_assert(numeric_limits<unsigned char>::digits == 8, "8-bit bytes (chars) are supported only.");

namespace
{
	static const char base64EncodeTable[] = {
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '+', '/' };

	inline void encodeTriplet(const char * const src, char * const dest) noexcept
	{
		const size_t pos1 = src[0] >> 2;
		assert(pos1 < 64);
		const size_t pos2 = ((src[0] & 0x03) << 4) + ((src[1] & 0xf0) >> 4);
		assert(pos2 < 64);
		const size_t pos3 = ((src[1] & 0x0f) << 2) + ((src[2] & 0xc0) >> 6);
		assert(pos3 < 64);
		const size_t pos4 = src[2] & 0x3f;
		assert(pos4 < 64);

		dest[0] = base64EncodeTable[pos1];
		dest[1] = base64EncodeTable[pos2];
		dest[2] = base64EncodeTable[pos3];
		dest[3] = base64EncodeTable[pos4];
	}
}

string afc::encodeBase64(const string &str)
{
	const size_t size = str.size();
	const size_t tailSize = size % 3;

	string dest;
	/* Each three octets are encoded into four octets.
	 * Pre-allocating space for them (including tailing '=' characters).
	 */
	dest.reserve((size + 2) / 3 * 4);

	char chunk[4];
	size_t i = 0;

	for (const size_t n = size - tailSize; i < n; i += 3) {
		encodeTriplet(&str[i], chunk);
		dest.append(chunk, 4);
	}

	// Last one or two octets are encoded in a different way.
	if (tailSize > 0) {
		char tail[3];
		tail[0] = str[i];
		tail[1] = tailSize == 1 ? 0 : str[i+1];
		tail[2] = 0;

		encodeTriplet(tail, chunk);

		if (tailSize == 1) {
			chunk[2] = '=';
		}
		chunk[3] = '=';

		dest.append(chunk, 4);
	}

	return dest;
}
