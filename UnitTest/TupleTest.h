
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
		Tuple tuple;
		vector<TupleHeader> tupleHeader = { TupleHeader("Name", TupleHeader::eType::_STRING),
			TupleHeader("Age", TupleHeader::eType::_INT),
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
			assert(tuple.end() == 2);
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
			Garbage<Tuple> gc;

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

		{
			Garbage<Tuple> gc;

			pTuple = tuple.where(vector<string>({ "Age" }),
				[](map<string, TupleValue> &mapCondition){ return mapCondition["Age"].getIntValue() < 30; })
				->select(vector<string>({ "Name", "Age", "Mobile" }))
				->top(1);
			assert(pTuple != NULL && pTuple != &tuple);
			assert(pTuple->getCount() == 1);
			pTuple->toFirst();
			assert((*pTuple)["Name"].getStringValue() == "AAA");
			assert((*pTuple)["Age"].getIntValue() == 25);
			assert((*pTuple)["Mobile"].getStringValue() == "111111");

			gc.add(pTuple);

			pTuple = tuple.where(vector<string>({ "Name" }),
				[](map<string, TupleValue> &mapCondition){ return mapCondition["Name"].getStringValue() != "AAA"; })
				->select(vector<string>({ "Name", "Age", "Mobile" }))
				->top(2);
			assert(pTuple != NULL && pTuple != &tuple);
			assert(pTuple->getCount() == 2);
			pTuple->toLast();
			assert((*pTuple)["Name"].getStringValue() == "CCCC");
			assert((*pTuple)["Age"].getIntValue() == 30);
			assert((*pTuple)["Mobile"].getStringValue() == "33333");

			gc.add(pTuple);
		}

		{
			Garbage<Tuple> gc;


			pTuple = tuple.where(vector<string>({}), nullptr)
				->select(vector<string>({ "Name", "Age", "Mobile" }));

			gc.add(pTuple);

			pTuple->where(vector<string>({ "Name" }),
				[](map<string, TupleValue> &mapCondition){ return mapCondition["Name"].getStringValue() == "BBBB"; })
				->update(vector<TupleValue>({ TupleValue("Age", 21), TupleValue("Mobile", "222222333") }));

			pTuple = pTuple->where(vector<string>({ "Name" }),
				[](map<string, TupleValue> &mapCondition){ return mapCondition["Name"].getStringValue() == "BBBB"; })
				->select(vector<string>({ "Name", "Age", "Mobile" }));

			gc.add(pTuple);

			assert(pTuple != NULL && pTuple != &tuple);
			assert(pTuple->getCount() == 1);
			pTuple->toFirst();
			assert((*pTuple)["Name"].getStringValue() == "BBBB");
			assert((*pTuple)["Age"].getIntValue() == 21);
			assert((*pTuple)["Mobile"].getStringValue() == "222222333");
		}

		{
			Garbage<Tuple> gc;

			pTuple = tuple.where(vector<string>({}), nullptr)
				->select(vector<string>({ "Name", "Age", "Mobile" }));
			assert(pTuple != NULL && pTuple != &tuple);
			assert(pTuple->getCount() == 3);

			gc.add(pTuple);

			pTuple->where(vector<string>({ "Name" }),
				[](map<string, TupleValue> &mapCondition){ return mapCondition["Name"].getStringValue() == "BBBB"; })
				->my_delete();

			assert(pTuple != NULL && pTuple != &tuple);
			assert(pTuple->getCount() == 2);
			pTuple->toFirst();
			assert((*pTuple)["Name"].getStringValue() == "AAA");
			assert((*pTuple)["Age"].getIntValue() == 25);
			assert((*pTuple)["Mobile"].getStringValue() == "111111");
			pTuple->toNext();
			assert((*pTuple)["Name"].getStringValue() == "CCCC");
			assert((*pTuple)["Age"].getIntValue() == 30);
			assert((*pTuple)["Mobile"].getStringValue() == "33333");

			pTuple->where(vector<string>({ }), nullptr)
				->my_delete();
			assert(pTuple->getCount() == 0);
		}


		{
			Garbage<Tuple> gc;

			pTuple = tuple.where(vector<string>({}), nullptr)
				->select(vector<string>({ "Name", "Age", "Mobile" }))
				->order_by("Age", "asc");

			gc.add(pTuple);

			assert(pTuple != NULL && pTuple != &tuple);
			assert(pTuple->getCount() == 3);
			pTuple->toFirst();
			assert((*pTuple)["Name"].getStringValue() == "BBBB");
			assert((*pTuple)["Age"].getIntValue() == 23);
			assert((*pTuple)["Mobile"].getStringValue() == "222222");
			pTuple->toNext();
			assert((*pTuple)["Name"].getStringValue() == "AAA");
			assert((*pTuple)["Age"].getIntValue() == 25);
			assert((*pTuple)["Mobile"].getStringValue() == "111111");
			pTuple->toNext();
			assert((*pTuple)["Name"].getStringValue() == "CCCC");
			assert((*pTuple)["Age"].getIntValue() == 30);
			assert((*pTuple)["Mobile"].getStringValue() == "33333");
		}

		{
			Garbage<Tuple> gc;

			pTuple = tuple.where(vector<string>({}), nullptr)
				->select(vector<string>({ "Name", "Age", "Mobile" }))
				->order_by("Name", "desc");

			gc.add(pTuple);

			assert(pTuple != NULL && pTuple != &tuple);
			assert(pTuple->getCount() == 3);
			pTuple->toFirst();
			assert((*pTuple)["Name"].getStringValue() == "CCCC");
			assert((*pTuple)["Age"].getIntValue() == 30);
			assert((*pTuple)["Mobile"].getStringValue() == "33333");
			pTuple->toNext();
			assert((*pTuple)["Name"].getStringValue() == "BBBB");
			assert((*pTuple)["Age"].getIntValue() == 23);
			assert((*pTuple)["Mobile"].getStringValue() == "222222");
			pTuple->toNext();
			assert((*pTuple)["Name"].getStringValue() == "AAA");
			assert((*pTuple)["Age"].getIntValue() == 25);
			assert((*pTuple)["Mobile"].getStringValue() == "111111");
		}
	}
	{
		Tuple tuple;
		vector<TupleHeader> tupleHeader = { TupleHeader("Name", TupleHeader::eType::_STRING),
			TupleHeader("Age", TupleHeader::eType::_INT),
			TupleHeader("Mobile", TupleHeader::eType::_STRING) };
		vector<Person> person = { Person("AAA", 25, "111111"),
			Person("BBBB", 23, "222222"),
			Person("CCCC", 30, "33333"),
			Person("DDDDD", 23, "234234234"),
			Person("EEEEE", 30, "3323423423333")
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
		}

		Tuple *pTuple = NULL;
		{
			Garbage<Tuple> gc;

			pTuple = tuple.where(vector<string>({}), nullptr)
				->select(vector<string>({ "Name", "Age", "Mobile" }))
				->gruop_by("Age");

			gc.add(pTuple);

			assert(pTuple != NULL && pTuple != &tuple);
			assert(pTuple->getCount() == 5);
			pTuple->toFirst();
			assert((*pTuple)["Name"].getStringValue() == "BBBB");
			assert((*pTuple)["Age"].getIntValue() == 23);
			assert((*pTuple)["Mobile"].getStringValue() == "222222");
			pTuple->toNext();
			assert((*pTuple)["Name"].getStringValue() == "DDDDD");
			assert((*pTuple)["Age"].getIntValue() == 23);
			assert((*pTuple)["Mobile"].getStringValue() == "234234234");
			pTuple->toNext();
			assert((*pTuple)["Name"].getStringValue() == "AAA");
			assert((*pTuple)["Age"].getIntValue() == 25);
			assert((*pTuple)["Mobile"].getStringValue() == "111111");
			pTuple->toNext();
			assert((*pTuple)["Name"].getStringValue() == "CCCC");
			assert((*pTuple)["Age"].getIntValue() == 30);
			assert((*pTuple)["Mobile"].getStringValue() == "33333");
			pTuple->toNext();
			assert((*pTuple)["Name"].getStringValue() == "EEEEE");
			assert((*pTuple)["Age"].getIntValue() == 30);
			assert((*pTuple)["Mobile"].getStringValue() == "3323423423333");
		}
		{
			Garbage<Tuple> gc;

			pTuple = tuple.where(vector<string>({ "Age" }),
				[](map<string, TupleValue> &mapCondition){ return mapCondition["Age"].getIntValue() < 30; })
				->select(vector<string>({ "Name", "Age", "Mobile" }))
				->gruop_by("Age");

			gc.add(pTuple);

			assert(pTuple != NULL && pTuple != &tuple);
			assert(pTuple->getCount() == 3);
			pTuple->toFirst();
			assert((*pTuple)["Name"].getStringValue() == "BBBB");
			assert((*pTuple)["Age"].getIntValue() == 23);
			assert((*pTuple)["Mobile"].getStringValue() == "222222");
			pTuple->toNext();
			assert((*pTuple)["Name"].getStringValue() == "DDDDD");
			assert((*pTuple)["Age"].getIntValue() == 23);
			assert((*pTuple)["Mobile"].getStringValue() == "234234234");
			pTuple->toNext();
			assert((*pTuple)["Name"].getStringValue() == "AAA");
			assert((*pTuple)["Age"].getIntValue() == 25);
			assert((*pTuple)["Mobile"].getStringValue() == "111111");
		}

	}
}


#endif
