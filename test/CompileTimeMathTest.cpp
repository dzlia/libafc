#include "CompileTimeMathTest.h"
#include <afc/compile_time_math.h>
#include <climits>
#include <limits>

afc::CompileTimeMathTest::CompileTimeMathTest()
{
	TEST_ADD(CompileTimeMathTest::testBitCount);
	TEST_ADD(CompileTimeMathTest::testOnesCount);
	TEST_ADD(CompileTimeMathTest::testLeadZeroCount);
	TEST_ADD(CompileTimeMathTest::testLog2);
}

void afc::CompileTimeMathTest::testBitCount()
{
	TEST_ASSERT(bitCount(0) == 1);
	TEST_ASSERT(bitCount(1) == 1);
	TEST_ASSERT(bitCount(0xff) == 8);
	TEST_ASSERT(bitCount(0x7f) == 7);
	TEST_ASSERT(bitCount(0x20) == 6);
	TEST_ASSERT(bitCount(0x1234) == 13);
}

void afc::CompileTimeMathTest::testOnesCount()
{
	TEST_ASSERT(onesCount(0) == 0);
	TEST_ASSERT(onesCount(1) == 1);
	TEST_ASSERT(onesCount(2) == 1);
	TEST_ASSERT(onesCount(3) == 2);
	TEST_ASSERT(onesCount(4) == 1);
	TEST_ASSERT(onesCount(5) == 2);
	TEST_ASSERT(onesCount(6) == 2);
	TEST_ASSERT(onesCount(7) == 3);
	TEST_ASSERT(onesCount(0xff) == 8);
	TEST_ASSERT(onesCount(0x100) == 1);
}

void afc::CompileTimeMathTest::testLeadZeroCount()
{
	TEST_ASSERT(leadZeroCount(0u) == unsigned(std::numeric_limits<unsigned>::digits));
	TEST_ASSERT(leadZeroCount(1u) == std::numeric_limits<unsigned>::digits - 1);
	TEST_ASSERT(leadZeroCount(UINT_MAX) == 0);
	TEST_ASSERT(leadZeroCount(0xf0u) == std::numeric_limits<unsigned>::digits - 8);
}

void afc::CompileTimeMathTest::testLog2()
{
	TEST_ASSERT(log2Floor(1u) == 0);
	TEST_ASSERT(log2Ceil(1u) == 0);
	TEST_ASSERT(log2Floor(2u) == 1);
	TEST_ASSERT(log2Ceil(2u) == 1);
	TEST_ASSERT(log2Floor(4u) == 2);
	TEST_ASSERT(log2Ceil(4u) == 2);
	TEST_ASSERT(log2Floor(16u) == 4);
	TEST_ASSERT(log2Ceil(16u) == 4);
	TEST_ASSERT(log2Floor(15u) == 3);
	TEST_ASSERT(log2Ceil(15u) == 4);
	TEST_ASSERT(log2Floor(17u) == 4);
	TEST_ASSERT(log2Ceil(17u) == 5);
}
