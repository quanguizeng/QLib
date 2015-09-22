
#ifndef QLIB_DELEGATETEST
#define QLIB_DELEGATETEST

#include "UnitTest.h"
#include "../Common/Delegate.h"
#include <iostream>

using namespace QLib;


class CParam
{
public:
	CParam(int main, int sub)
	{
		mMain = main;
		mSub = sub;
		mValue = 0;
	}
	~CParam()
	{

	}

public:
	int mMain;
	int mSub;
	int mValue;
};

class CReturn
{
public:
	CReturn()
	{
		mFlag = false;
		mValue = 0;
	}
	~CReturn()
	{

	}

public:
	bool mFlag;
	int mValue;
};

class CTest1
{
public:
	CTest1(int main, int sub)
	{
		mMain = main;
		mSub = sub;
	}
	~CTest1()
	{

	}

	void myFunc(void* pParams, void *pReturn)
	{
		TEST_ASSERT(pParams);
		TEST_ASSERT(pReturn);
		if (pParams == NULL || pReturn == NULL)
		{
			return;
		}
		CParam *pParams2 = (CParam*)pParams;
		CReturn *pReturn2 = (CReturn*)pReturn;
		pReturn2->mFlag = true;
		if (pParams2->mMain == mMain && pParams2->mSub == mSub)
		{
			pReturn2->mValue = 100;
			cout << "CTest1::myFunc" << endl;
		}
	}

public:
	int mMain;
	int mSub;
};

class Test2
{
public:
	Test2(int main, int sub)
	{
		mMain = main;
		mSub = sub;
	}
	~Test2()
	{

	}

	void myFunc(void* pParams, void *pReturn)
	{
		if (pParams == NULL || pReturn == NULL)
		{
			return;
		}
		CParam *pParams2 = (CParam*)pParams;
		CReturn *pReturn2 = (CReturn*)pReturn;

		pReturn2->mFlag = true;
		if (pParams2->mMain == mMain && pParams2->mSub == mSub)
		{
			pReturn2->mValue = 200;
			cout << "Test2::myFunc" << endl;
		}
	}

private:
	int mMain;
	int mSub;
};

void func(int x, int y)
{
	cout << "func(" << x << ", " << y << ")" << endl;
}

void func2(int x, int y)
{
	cout << "func2(" << x << ", " << y << ")" << endl;
}

class NormalClass
{
public:
	void normalFunc(int x, int y)
	{
		cout << "NormalClass::normalFunc(" << x << ", " << y << ")" << endl;
	}
};

class BaseClass
{
public:
	virtual void virFunc(int x, int y)
	{
		cout << "BaseClass::virFunc(" << x << ", " << y << ")" << endl;
	}

	void normalFunc(int x, int y)
	{
		cout << "BaseClass::normalFunc(" << x << ", " << y << ")" << endl;
	}
};

class DerivedClass : public BaseClass
{
public:
	virtual void virFunc(int x, int y)
	{
		cout << "DerivedClass::virFunc(" << x << ", " << y << ")" << endl;
	}

	static void staticFunc(int x, int y)
	{
		cout << "DerivedClass::staticFunc(" << x << ", " << y << ")" << endl;
	}
};

template<class T>
void tFunc(T x, T y)
{
	cout << "tFunc(" << x << ", " << y << ")" << endl;
}

TEST_CASE(testDelegate)
{
	{
		typedef CMultiDelegate<void*, void*> EvenetHandler;
		EvenetHandler event;

		CTest1 *pTest1 = new CTest1(1, 1);
		Test2 *pTest2 = new Test2(2,1);
		CParam *pParam = new CParam(1, 1);
		CReturn *pReturn = new CReturn();

		event += createDelegate(pTest1, &CTest1::myFunc);
		event += createDelegate(pTest2, &Test2::myFunc);

		event((void*)pParam, (void*)pReturn);
		TEST_ASSERT(pReturn->mFlag == true);
		TEST_ASSERT(pReturn->mValue == 100);

		pParam->mMain = 2;
		pParam->mSub = 1;
		event((void*)pParam, (void*)pReturn);
		TEST_ASSERT(pReturn->mFlag == true);
		TEST_ASSERT(pReturn->mValue == 200);

		delete pTest1;
		delete pTest2;
		delete pParam;
		delete pReturn;
	}

	{
		typedef CMultiDelegate<int, int> EvenetHandler;
		EvenetHandler event;

		BaseClass *pBaseClass = new BaseClass;
		DerivedClass *pDerivedClass = new DerivedClass;
		NormalClass *pNormalClass = new NormalClass;

		event += createDelegate(func);
		event += createDelegate(func2);

		event += createDelegate(pNormalClass, &NormalClass::normalFunc);
		event += createDelegate(pBaseClass, &BaseClass::normalFunc);

		event += createDelegate(pBaseClass, &BaseClass::virFunc);
		event += createDelegate(pDerivedClass, &DerivedClass::virFunc);
		// 多态下，使用基类指针时，函数指针要用基类的函数指针，但是在调用时会响应多态，调用派生类的虚函数
		event += createDelegate((BaseClass*)pDerivedClass, &BaseClass::virFunc);

		event += createDelegate(&DerivedClass::staticFunc);

		event += createDelegate(tFunc<int>);

		event(10, 20);
		cout << endl;

		event -= createDelegate(func);
		event -= createDelegate(pNormalClass, &NormalClass::normalFunc);

		event(20, 30);
		cout << endl;

		delete pBaseClass;
		delete pDerivedClass;
		delete pNormalClass;
	}
}

#endif