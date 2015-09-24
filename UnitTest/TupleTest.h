
#ifndef QLIB_TUPLE_TEST
#define QLIB_TUPLE_TEST

#include "../common/Tuple.h"
#include "../common/Garbage.h"
#include "UnitTest.h"
#include<string>
#include<list>
#include<assert.h>
#include<vector>
#include <map>

using namespace std;

using namespace QLib;


class Person
{
public:
	Person(string name, int age, string mobile)
	{
		mName = name;
		mAge = age;
		mMobile = mobile;
	}
	~Person()
	{
	}

	inline string getName()
	{
		return mName;
	}
	inline int getAge()
	{
		return mAge;
	}
	inline string getMobile()
	{
		return mMobile;
	}

	string mName;
	int mAge;
	string mMobile;
};


TEST_CASE(testTuple)
{
	{
		Garbage<Tuple> gc;
		Tuple tuple;
		vector<TupleHeader> tupleHeader = {TupleHeader("Name", TupleHeader::eType::_STRING),
			TupleHeader("Age", TupleHeader::eType::_INT) ,
			TupleHeader("Mobile", TupleHeader::eType::_STRING) };
		vector<Person> person = { Person("AAA", 25, "111111"),
			Person("BBBB", 23, "222222"),
			Person("CCCC", 30, "33333")
		};

		{
			tuple.create(tupleHeader);

			for (vector<Person>::iterator i = person.begin(); i != person.end(); i++)
			{
				vector<TupleValue> tupleListValue = { TupleValue(tupleHeader[0].getName(), (*i).mName),
					TupleValue(tupleHeader[1].getName(), (*i).mAge),
					TupleValue(tupleHeader[2].getName(), (*i).mMobile)
					};
				tuple.insert(tupleListValue);
			}

			tuple.toFirst();
			assert(tuple.begin() == 0);
			tuple.toNext();
			assert(tuple.getCurPos() == 1);
			assert(tuple.end() == 3);
			assert(tuple.getCount() == 3);

			tuple.toFirst();
			for (int i = tuple.begin(); i != tuple.end(); i++)
			{
				assert(tuple["Name"].getStringValue() == person[i].getName());
				assert(tuple["Age"].getIntValue() == person[i].getAge());
				assert(tuple["Mobile"].getStringValue() == person[i].getMobile());
				tuple.toNext();
			}
		}

		Tuple *pTuple = NULL;
		{
			pTuple = tuple.where(vector<string>({ "Name" }),
				[](map<string, TupleValue> &mapCondition){ return mapCondition["Name"].getStringValue() == "CCCC"; })
				->select(vector<string>({ "Name", "Age", "Mobile" }));
			assert(pTuple != NULL && pTuple != &tuple);
			assert(pTuple->getCount() == 1);
			pTuple->toFirst();
			assert((*pTuple)["Name"].getStringValue() == "CCCC");
			assert((*pTuple)["Age"].getIntValue() == 30);
			assert((*pTuple)["Mobile"].getStringValue() == "33333");

			gc.add(pTuple);
		}

	}

}


#endif
