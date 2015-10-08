#ifndef QLIB_REFLECTION_TEST
#define QLIB_REFLECTION_TEST

#include "../Common/Reflection.h"
#include "UnitTest.h"

using namespace QLib;

class TestClass
{
	DEFINE_DYNAMIC_CREATE(TestClass);
public:
	TestClass()
	{
		x = 100;
		y = 200;
	}
	int x;
	int y;
};

IMPLIMENT_DYNAMIC_CREATE(TestClass);

TEST_CASE(testReflection)
{
	Garbage<ClassFactory> classFactoryGC(ClassFactoryInstance());

	Garbage<TestClass> testGC;
	TestClass *pTest = (TestClass*)ClassFactoryInstance()->getClassByName("TestClass");
	assert(pTest != nullptr);
	testGC.add(pTest);
}


#endif