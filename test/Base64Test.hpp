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
#ifndef AFC_BASE64TEST_H_
#define AFC_BASE64TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	// TODO Add encode tests that test all possible values of an octet.
	class Base64Test : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(Base64Test);
		CPPUNIT_TEST(testEncodeBase64String_EmptyString);
		CPPUNIT_TEST(testEncodeBase64String_Octet);
		CPPUNIT_TEST(testEncodeBase64String_TwoOctets);
		CPPUNIT_TEST(testEncodeBase64String_Triplet);
		CPPUNIT_TEST(testEncodeBase64String_TripletAndOctet);
		CPPUNIT_TEST(testEncodeBase64String_TripletAndTwoOctets);
		CPPUNIT_TEST(testEncodeBase64String_TwoTriplets);
		CPPUNIT_TEST(testEncodeBase64String_TwoTripletsAndOctet);
		CPPUNIT_TEST(testEncodeBase64String_TwoTripletsAndTwoOctets);
		CPPUNIT_TEST(testBase64Size_Unsigned);
		CPPUNIT_TEST(testBase64Size_UnsignedLong);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testEncodeBase64String_EmptyString();
		void testEncodeBase64String_Octet();
		void testEncodeBase64String_TwoOctets();
		void testEncodeBase64String_Triplet();
		void testEncodeBase64String_TripletAndOctet();
		void testEncodeBase64String_TripletAndTwoOctets();
		void testEncodeBase64String_TwoTriplets();
		void testEncodeBase64String_TwoTripletsAndOctet();
		void testEncodeBase64String_TwoTripletsAndTwoOctets();
		void testBase64Size_Unsigned();
		void testBase64Size_UnsignedLong();
	};
}

#endif /* AFC_BASE64TEST_H_ */
