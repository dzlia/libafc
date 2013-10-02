#include "UtilsTest.h"
#include <afc/utils.h>
#include <functional>

using std::string;
using std::vector;

afc::UtilsTest::UtilsTest()
{
	TEST_ADD(UtilsTest::testTrim);
	TEST_ADD(UtilsTest::testStartsWith);
	TEST_ADD(UtilsTest::testEndsWith);
	TEST_ADD(UtilsTest::testBinarySearch);
}

void afc::UtilsTest::testTrim()
{
	{
		string s;
		trim(s);
		TEST_ASSERT(s.empty());
	}
	{
		string s("asd");
		trim(s);
		TEST_ASSERT(s == "asd");
	}
	{
		string s("\t  \n\r   hello \r\t   world   ");
		trim(s);
		TEST_ASSERT(s == "hello \r\t   world");
	}
	{
		string s("   \t\t\t\t");
		trim(s);
		TEST_ASSERT(s.empty());
	}
	{
		string s("\t  \n\r   hello \r\t   world");
		trim(s);
		TEST_ASSERT(s == "hello \r\t   world");
	}
	{
		string s("hello \r\t   world   ");
		trim(s);
		TEST_ASSERT(s == "hello \r\t   world");
	}
	{
		string s("\t  \n\r   hello \r\t   world\t");
		trim(s);
		TEST_ASSERT(s == "hello \r\t   world");
	}
	{
		string s("\tworld   ");
		trim(s);
		TEST_ASSERT(s == "world");
	}
}

void afc::UtilsTest::testStartsWith()
{
	TEST_ASSERT(startsWith("str", "s"));
	TEST_ASSERT(startsWith("str", "st"));
	TEST_ASSERT(startsWith("str", "str"));
	TEST_ASSERT(!startsWith("str", "str2"));
	TEST_ASSERT(!startsWith("str", "sTr"));
	TEST_ASSERT(!startsWith("str", "S"));
	TEST_ASSERT(startsWith("str", ""));
	TEST_ASSERT(startsWith("", ""));
	TEST_ASSERT(!startsWith("", "sWW"));
	TEST_ASSERT(!startsWith("z", "s"));
	TEST_ASSERT(!startsWith("z", "zz"));
	TEST_ASSERT(startsWith("z", "z"));
	TEST_ASSERT(startsWith("z", ""));
}

void afc::UtilsTest::testEndsWith()
{
	TEST_ASSERT(endsWith("str", "r"));
	TEST_ASSERT(endsWith("str", "tr"));
	TEST_ASSERT(endsWith("str", "str"));
	TEST_ASSERT(endsWith("str", ""));
	TEST_ASSERT(!endsWith("str", "rt"));
	TEST_ASSERT(!endsWith("str", "astr"));
	TEST_ASSERT(endsWith("s", ""));
	TEST_ASSERT(endsWith("s", "s"));
	TEST_ASSERT(!endsWith("s", "z"));
	TEST_ASSERT(!endsWith("s", "ss"));
	TEST_ASSERT(endsWith("", ""));
	TEST_ASSERT(!endsWith("", "z"));
	TEST_ASSERT(!endsWith("", "ss"));
}

void afc::UtilsTest::testBinarySearch()
{
	std::less<int> less;
	{
		vector<int> a;
		for (int i = 0; i < 10; ++i) {
			a.push_back(i);
		}
		for (int i = 9; i >= 0; --i) {
			size_t pos;
			const bool success = binarySearch(a, i, less, pos);
			TEST_ASSERT(success);
			TEST_ASSERT(pos == static_cast<unsigned>(i));
		}
		size_t pos;
		TEST_ASSERT((!binarySearch(a, -2, less, pos)));
		TEST_ASSERT((!binarySearch(a, 10, less, pos)));
		TEST_ASSERT((!binarySearch(a, 12, less, pos)));
	}

	{
		vector<int> a;
		for (int i = 0; i < 20; i += 2) {
			a.push_back(i);
		}
		for (int i = 18; i >= 0; i -= 2) {
			size_t pos;
			const bool success = binarySearch(a, i, less, pos);
			TEST_ASSERT(success);
			TEST_ASSERT(pos == static_cast<unsigned>(i/2));
		}
	}

	{
		vector<int> a;
		for (int i = 18; i >= 0; i -= 2) {
			size_t pos;
			const bool success = binarySearch(a, i, less, pos);
			TEST_ASSERT(!success);
		}
	}

	{
		vector<int> a;
		a.push_back(5);
		size_t pos;
		TEST_ASSERT(binarySearch(a, 5, less, pos));
		TEST_ASSERT(pos == 0);

		TEST_ASSERT(!binarySearch(a, 4, less, pos));
		TEST_ASSERT(!binarySearch(a, 6, less, pos));
		TEST_ASSERT(!binarySearch(a, 0, less, pos));
	}
}
