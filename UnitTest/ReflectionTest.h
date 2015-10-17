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
	Garbage<TestClass> testGC;
	TestClass *pTest = (TestClass*)ClassFactoryInstance()->getClassByName("TestClass");
	assert(pTest != nullptr);
	testGC.add(pTest);
}

class TestClass2 :public MmemberBase
{
	DEFINE_DYNAMIC_CREATE(TestClass2);
	MEMBER_INTERFACE(TestClass2, int, mX);
	MEMBER_INTERFACE(TestClass2, int, mY);
	MEMBER_INTERFACE(TestClass2, string, mName);

public:
	TestClass2()
	{
		registProperty();
	}
	~TestClass2()
	{
	}

	void registProperty()
	{
		ADD_SET_MEMBER_INTERFACE(TestClass2, this, mX);
		ADD_SET_MEMBER_INTERFACE(TestClass2, this, mY);
		ADD_SET_MEMBER_INTERFACE(TestClass2, this, mName);

		ADD_GET_MEMBER_INTERFACE(TestClass2, this, mX);
		ADD_GET_MEMBER_INTERFACE(TestClass2, this, mY);
		ADD_GET_MEMBER_INTERFACE(TestClass2, this, mName);
	}

	int mX;
	int mY;
	string mName;
};
IMPLIMENT_DYNAMIC_CREATE(TestClass2);

TEST_CASE(testMemberValue)
{
	{
		TestClass2 *pTestClass2 = (TestClass2*)ClassFactoryInstance()->getClassByName("TestClass2");
		Garbage<TestClass2> testGC;

		testGC.add(pTestClass2);

		int x = 100;
		int y = 200;
		string name = "AAAAA";

		SET_MEMBER_VALUE(pTestClass2, mX, x);
		SET_MEMBER_VALUE(pTestClass2, mY, y);
		SET_MEMBER_VALUE(pTestClass2, mName, name);

		int myX;
		int myY;
		string myName;

		GET_MEMBER_VALUE(pTestClass2, myX, mX);
		GET_MEMBER_VALUE(pTestClass2, myY, mY);
		GET_MEMBER_VALUE(pTestClass2, myName, mName);

		assert(myX == 100);
		assert(myY == 200);
		assert(myName == "AAAAA");
	}
}

#endif