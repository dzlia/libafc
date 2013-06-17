//============================================================================
// Name        : stack_trace.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdexcept>
#include <cpptest.h>
#include "CompileTimeMathTest.h"
#include "FastDivisionTest.h"
#include "FileTest.h"
#include "MathUtilsTest.h"
#include "NumberTest.h"
#include "RepositoryTest.h"
#include "TokeniserTest.h"
#include "UtilsTest.h"
#include "cpu/Int32Test.h"

using namespace std;
using namespace afc;
using namespace Test;

int main() {
	TextOutput out(TextOutput::Verbose);
	Suite allTests;
	allTests.add(auto_ptr<Suite>(new CompileTimeMathTest));
	allTests.add(auto_ptr<Suite>(new FastDivisionTest));
	allTests.add(auto_ptr<Suite>(new FileTest));
	allTests.add(auto_ptr<Suite>(new MathUtilsTest));
	allTests.add(auto_ptr<Suite>(new NumberTest));
	allTests.add(auto_ptr<Suite>(new RepositoryTest));
	allTests.add(auto_ptr<Suite>(new TokeniserTest));
	allTests.add(auto_ptr<Suite>(new UtilsTest));

	allTests.add(auto_ptr<Suite>(new Int32Test));
	allTests.run(out);
	return 0;
}
