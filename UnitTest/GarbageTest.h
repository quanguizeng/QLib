
#ifndef QLIB_GARBAGE_TEST
#define QLIB_GARBAGE_TEST

#include "../common/Garbage.h"
#include "UnitTest.h"

using namespace QLib;

class Test1
{
public:
	Test1()
	{
		x = 100;
	}
	~Test1()
	{

	}
	int x;
	int y;
	int z;
};



TEST_CASE(testGarbage)
{
	{
		Garbage<int> gc;
		int *pInt = new int[1024];
		gc.add(pInt);
		TEST_ASSERT(gc.size() == 1);
	}

	{
		Garbage<int> gc;
		int *pInt = new int[1024];
		gc.add(pInt);
		gc.destroy(pInt);
		TEST_ASSERT(gc.size() == 0);
	}

	{
		Garbage<int> gc;
		int *pInt1 = new int[1024];
		int *pInt2 = new int[1024];
		gc.add(pInt1);
		gc.add(pInt2);
		gc.destroy(pInt1);
		TEST_ASSERT(gc.size() == 1);
	}

	{
		Garbage<Test1> gc;
		Test1 *pTest = new Test1();
		gc.add(pTest);
		gc.destroy(pTest);
		TEST_ASSERT(gc.size() == 0);
	}
}

#endif