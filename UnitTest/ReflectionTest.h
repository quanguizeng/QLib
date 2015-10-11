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
	}
	~TestClass2()
	{
	}

	void registProperty()
	{
		getPropertyList().insert(pair<string, setMemberValue>(string("setmX"), TestClass2::setmX));
		getPropertyList().insert(pair<string, setMemberValue>(string("setmY"), TestClass2::setmY));
		getPropertyList().insert(pair<string, setMemberValue>(string("setmName"), TestClass2::setmName));
		getPropertyList().insert(pair<string, setMemberValue>(string("getmX"), TestClass2::getmX));
		getPropertyList().insert(pair<string, setMemberValue>(string("getmY"), TestClass2::getmY));
		getPropertyList().insert(pair<string, setMemberValue>(string("getmName"), TestClass2::getmName));
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

		pTestClass2->registProperty();

		int x = 100;
		int y = 200;
		string name = "AAAAA";
		pTestClass2->getPropertyList()["setmX"](pTestClass2, &x);
		pTestClass2->getPropertyList()["setmY"](pTestClass2, &y);
		pTestClass2->getPropertyList()["setmName"](pTestClass2, &name);

		int myX;
		int myY;
		string myName;

		pTestClass2->getPropertyList()["getmX"](pTestClass2, &myX);
		pTestClass2->getPropertyList()["getmY"](pTestClass2, &myY);
		pTestClass2->getPropertyList()["getmName"](pTestClass2, &myName);

		assert(myX == 100);
		assert(myY == 200);
		assert(myName == name);
	}
}

#endif