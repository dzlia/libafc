/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2023 Dźmitry Laŭčuk

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
#include "Base64Test.hpp"
#include <afc/base64.hpp>
#include <string>
#include <iterator>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::Base64Test);

void afc::Base64Test::testEncodeBase64String_EmptyString()
{
	string result;
	encodeBase64("", 0, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string(), result);
}

void afc::Base64Test::testEncodeBase64String_Octet()
{
	string result;
	encodeBase64("M", 1, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("TQ=="), result);
}

void afc::Base64Test::testEncodeBase64String_TwoOctets()
{
	string result;
	encodeBase64("Ma", 2, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("TWE="), result);
}

void afc::Base64Test::testEncodeBase64String_Triplet()
{
	string result;
	encodeBase64("Man", 3, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("TWFu"), result);
}

void afc::Base64Test::testEncodeBase64String_TripletAndOctet()
{
	string result;
	encodeBase64("ManM", 4, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("TWFuTQ=="), result);
}

void afc::Base64Test::testEncodeBase64String_TripletAndTwoOctets()
{
	string result;
	encodeBase64("ManMa", 5, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("TWFuTWE="), result);
}

void afc::Base64Test::testEncodeBase64String_TwoTriplets()
{
	string result;
	encodeBase64("Triple", 6, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("VHJpcGxl"), result);
}

void afc::Base64Test::testEncodeBase64String_TwoTripletsAndOctet()
{
	string result;
	encodeBase64("TripleM", 7, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("VHJpcGxlTQ=="), result);
}

void afc::Base64Test::testEncodeBase64String_TwoTripletsAndTwoOctets()
{
	string result;
	encodeBase64("TripleMa", 8, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("VHJpcGxlTWE="), result);
}

void afc::Base64Test::testBase64Size_Unsigned()
{
	CPPUNIT_ASSERT_EQUAL(0u, afc::base64Size(0u));
	CPPUNIT_ASSERT_EQUAL(4u, afc::base64Size(1u));
	CPPUNIT_ASSERT_EQUAL(4u, afc::base64Size(2u));
	CPPUNIT_ASSERT_EQUAL(4u, afc::base64Size(3u));
	CPPUNIT_ASSERT_EQUAL(8u, afc::base64Size(4u));
	CPPUNIT_ASSERT_EQUAL(8u, afc::base64Size(5u));
	CPPUNIT_ASSERT_EQUAL(16u, afc::base64Size(10u));
}

void afc::Base64Test::testBase64Size_UnsignedLong()
{
	CPPUNIT_ASSERT_EQUAL(0ul, afc::base64Size(0ul));
	CPPUNIT_ASSERT_EQUAL(4ul, afc::base64Size(1ul));
	CPPUNIT_ASSERT_EQUAL(4ul, afc::base64Size(2ul));
	CPPUNIT_ASSERT_EQUAL(4ul, afc::base64Size(3ul));
	CPPUNIT_ASSERT_EQUAL(8ul, afc::base64Size(4ul));
	CPPUNIT_ASSERT_EQUAL(8ul, afc::base64Size(5ul));
	CPPUNIT_ASSERT_EQUAL(16ul, afc::base64Size(10ul));
}
