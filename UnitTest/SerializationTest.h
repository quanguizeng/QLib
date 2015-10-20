
#ifndef QLIB_SERIALIZATIONTEST
#define QLIB_SERIALIZATIONTEST

#include "UnitTest.h"
#include "../Common/Stream/Serialization.h"
#include <vector>
#include <set>
#include <list>
#include <map>
#include "../Common/Stream/MemoryStream.h"
#include "../Common/Reflection.h"

using namespace std;

TEST_CASE(testSerializationBaseType)
{
	{
		MemoryStream stream;

		assert(stream.size() == 0);

		writeStream<char>(stream, 'a');
		writeStream<int>(stream, 10);
		writeStream<double>(stream, 123.4567);
		writeStream<string>(stream, string("AAAAA"));

		assert(readStream<char>(stream) == 'a');
		assert(readStream<int>(stream) == 10);
		assert(readStream<double>(stream) == 123.4567);
		assert(readStream<string>(stream) == string("AAAAA"));
		assert(stream.size() == 0);
	}
}

TEST_CASE(testSerializationContainer)
{
	{
		MemoryStream stream;

		vector<int> list1 = { 10, 20, 30 };

		assert(stream.size() == 0);
		ContainerSerialization<int, vector<int>>::write(stream, list1);
		assert(stream.size() == sizeof(int) + sizeof(int)*list1.size());

		vector<int> list2;
		ContainerSerialization<int, vector<int>>::read(stream, list2);
		assert(stream.size() == 0);
		assert(list2.size() == list1.size());

		typedef vector<int>::const_iterator iterator;

		for (iterator it1 = list1.begin(), it2 = list2.begin(); it1 != list1.end(); it1++, it2++)
		{
			assert(*it1 == *it2);
		}
	}

	{
		MemoryStream stream;

		list<int> list1 = { 10, 20, 30 };

		assert(stream.size() == 0);
		ContainerSerialization<int, list<int>>::write(stream, list1);
		assert(stream.size() == sizeof(int) + sizeof(int)*list1.size());

		list<int> list2;
		ContainerSerialization<int, list<int>>::read(stream, list2);
		assert(stream.size() == 0);
		assert(list2.size() == list1.size());

		typedef list<int>::const_iterator iterator;

		for (iterator it1 = list1.begin(), it2 = list2.begin(); it1 != list1.end(); it1++,it2++)
		{
			assert(*it1 == *it2);
		}
	}

	{
		MemoryStream stream;

		set<int> list1 = { 10, 20, 30 };

		assert(stream.size() == 0);
		ContainerSerialization<int, set<int>>::write(stream, list1);
		assert(stream.size() == sizeof(int) + sizeof(int)*list1.size());

		set<int> list2;
		ContainerSerialization<int, set<int>>::read(stream, list2);
		assert(stream.size() == 0);
		assert(list2.size() == list1.size());

		typedef set<int>::const_iterator iterator;

		for (iterator it1 = list1.begin(), it2 = list2.begin(); it1 != list1.end(); it1++, it2++)
		{
			assert(*it1 == *it2);
		}
	}

	{
		MemoryStream stream;

		map<string, string> map1 = { pair<string, string>("A", string("AAAA")),
			pair<string, string>("B", string("BBBB")),
			pair<string, string>("C", string("CCCC")) };
		Garbage<string> gc;

		MapSerialization<string, string, map<string, string>>::write(stream, map1);

		map<string, string> map2;

		MapSerialization<string, string, map<string, string>>::read(stream, map2);
		assert(map1.size() == map2.size());

		typedef map<string, string>::const_iterator iterator;
		for (iterator it1 = map1.begin(), it2 = map2.begin(); it1 != map1.end(); it1++, it2++)
		{
			assert(it1->first == it2->first);
			assert(it1->second == it2->second);
		}
	}
}

class TestSerialization :public MemberBase, public ClassSerialization<TestSerialization>
{
	DEFINE_DYNAMIC_CREATE(TestSerialization);
	MEMBER_INTERFACE(TestSerialization, int, mX);
	MEMBER_INTERFACE(TestSerialization, int, mY);
	MEMBER_INTERFACE(TestSerialization, string, mName);

public:
	TestSerialization() :
		ClassSerialization<TestSerialization>(this)
	{
		registProperty();
	}
	~TestSerialization()
	{
	}

	void registProperty()
	{
		ADD_SET_MEMBER_INTERFACE(TestSerialization, this, mX);
		ADD_SET_MEMBER_INTERFACE(TestSerialization, this, mY);
		ADD_SET_MEMBER_INTERFACE(TestSerialization, this, mName);

		ADD_GET_MEMBER_INTERFACE(TestSerialization, this, mX);
		ADD_GET_MEMBER_INTERFACE(TestSerialization, this, mY);
		ADD_GET_MEMBER_INTERFACE(TestSerialization, this, mName);
	}

	virtual bool serialization(IStream &stream)
	{
		MemberBase *pObject = (MemberBase*)mObject;
		writeStream<int>(stream, mX);
		writeStream<int>(stream, mY);
		writeStream<string>(stream, mName);

		return true;
	}

	virtual bool deserialization(IStream &stream)
	{
		MemberBase *pObject = (MemberBase*)mObject;

		mX = readStream<int>(stream);
		mY = readStream<int>(stream);
		mName = readStream<string>(stream);

		return true;
	}

protected:
	int mX;
	int mY;
	string mName;
};

IMPLIMENT_DYNAMIC_CREATE(TestSerialization);

TEST_CASE(testClassSerialization)
{
	{
		TestSerialization *pObject1 = (TestSerialization*)ClassFactoryInstance()->getClassByName("TestSerialization");
		Garbage<TestSerialization> testGC;

		testGC.add(pObject1);

		int x = 100;
		int y = 200;
		string name = "AAAAA";

		SET_MEMBER_VALUE(pObject1, mX, x);
		SET_MEMBER_VALUE(pObject1, mY, y);
		SET_MEMBER_VALUE(pObject1, mName, name);

		MemoryStream stream;

		ClassSerializationBase *pBase = (ClassSerializationBase*)pObject1;
		pBase->serialization(stream);

		TestSerialization *pObject2 = (TestSerialization*)ClassFactoryInstance()->getClassByName("TestSerialization");
		testGC.add(pObject2);
		pBase = (ClassSerializationBase*)pObject2;
		pBase->deserialization(stream);

		int myX;
		int myY;
		string myName;

		GET_MEMBER_VALUE(pObject2, myX, mX);
		GET_MEMBER_VALUE(pObject2, myY, mY);
		GET_MEMBER_VALUE(pObject2, myName, mName);

		assert(myX == 100);
		assert(myY == 200);
		assert(myName == "AAAAA");
	}

}

#endif