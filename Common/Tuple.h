
//##########################################################################
//Qlib 1.0
//开发者:曾全贵
//##########################################################################

/*
This file is part of Qlib.

Qlib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Qlib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Qlib.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef QLIB_TUPLE
#define QLIB_TUPLE

#include <map>
#include <string>
#include <list>
#include<vector>
#include <typeinfo>
#include <assert.h>
#include <functional>
#include <algorithm>
#include "Garbage.h"
#include "Basic.h"

using namespace std;

namespace QLib
{

	class TupleHeader
	{
		enum eType;
	public:
		TupleHeader(string headerName, eType headerType)
		{
			mName = headerName;
			mType = headerType;
		}
		~TupleHeader()
		{

		}

		inline eType getType()
		{
			return mType;
		}
		inline string getName()
		{
			return mName;
		}
		inline bool isInt()
		{
			return mType == eType::_INT;
		}
		inline bool isString()
		{
			return mType == eType::_STRING;
		}
		inline bool isDouble()
		{
			return mType == eType::_DOUBLE;
		}
		typedef enum eType
		{
			_INT,
			_STRING,
			_DOUBLE
		};

	protected:
		string mName;
		eType mType;
	};

	class TupleValue
	{
	public:
		TupleValue()
		{

		}
		TupleValue(string headerName, string value)
		{
			mName = headerName;
			mStringValue = value;
		}
		TupleValue(string headerName, int value)
		{
			mName = headerName;
			mIntValue = value;
		}
		TupleValue(string headerName, double value)
		{
			mName = headerName;
			mDoubleValue = value;
		}
		~TupleValue()
		{

		}

		inline string getStringValue()
		{
			return mStringValue;
		}

		inline int getIntValue()
		{
			return mIntValue;
		}

		inline double getDoubleValue()
		{
			return mDoubleValue;
		}

		inline string getName()
		{
			return mName;
		}

	protected:
		string mName;
		string mStringValue;
		int mIntValue;
		double mDoubleValue;
	};

	class Tuple
	{
#define	_DEFAULT_		"_DEFAULT_"
	public:
		typedef	TupleHeader::eType	HeaderType;
		typedef map<string, void*> TupleTable;
		typedef function< bool(map<string, TupleValue>&)>	WhereCondition;
		class TupleType
		{
		public:
			typedef vector<double>	DoubleType;
			typedef vector<int>	IntType;
			typedef vector<string>	StringType;
		};

		Tuple()
		{
			mTable.clear();
			mCurrentPointer = 0;
			mFunc = nullptr;
		}
		~Tuple()
		{
		}

		bool create(vector<TupleHeader> &tupleHeader)
		{
			typedef vector<TupleHeader>::iterator iterator;

			mTupleHeader = tupleHeader;

			addIntList(_DEFAULT_);

			for (iterator it = tupleHeader.begin(); it != tupleHeader.end(); ++it)
			{
				TupleHeader &header = *it;
				if (header.isDouble())
				{
					addDoubleList(header.getName());
				}
				else if (header.isString())
				{
					addStringList(header.getName());
				}
				else if (header.isInt())
				{
					addIntList(header.getName());
				}
			}
			return true;
		}

		Tuple* select(vector<string> &listHeader)
		{
			CHECK_ERROR(listHeader.size() > 0, "");
			CHECK_ERROR(checkHeader(listHeader) == true, "");

			typedef vector<string>::iterator iterator;
			vector<TupleHeader> tupleHeader;
			for (iterator it = listHeader.begin(); it != listHeader.end(); it++)
			{
				tupleHeader.push_back(*getHeader(*it));
			}
			Tuple *pTuple = new Tuple();

			pTuple->create(tupleHeader);

			map<string, TupleValue> mapValue;
			for (int i = 0; i < getCount(); i++)
			{
				if (mFunc != nullptr)
				{
					for (vector<string>::iterator itHeader = mListHeader.begin(); itHeader != mListHeader.end(); itHeader++)
					{
						TupleHeader *pHeader = getHeader(*itHeader);
						CHECK_ERROR(pHeader != NULL, "查询的字段不存在");
						if (pHeader->isDouble())
						{
							TupleType::DoubleType *pDoubleField = (TupleType::DoubleType*)mTable[*itHeader];
							mapValue[*itHeader] = TupleValue(pHeader->getName(), (*pDoubleField)[i]);
						}
						else if (pHeader->isInt())
						{
							TupleType::IntType *pDoubleField = (TupleType::IntType*)mTable[*itHeader];
							mapValue[*itHeader] = TupleValue(pHeader->getName(), (*pDoubleField)[i]);
						}
						else if (pHeader->isString())
						{
							TupleType::StringType *pDoubleField = (TupleType::StringType*)mTable[*itHeader];
							mapValue[*itHeader] = TupleValue(pHeader->getName(), (*pDoubleField)[i]);
						}
					}
				}

				vector<TupleValue>	listValue;
				if (mFunc(mapValue))
				{
					for (vector<string>::iterator it = listHeader.begin(); it != listHeader.end(); it++)
					{
						TupleHeader *pHeader = getHeader(*it);
						if (pHeader->isDouble())
						{
							TupleType::DoubleType *pFiled = (TupleType::DoubleType*)mTable[*it];
							listValue.push_back(TupleValue(pHeader->getName(), (*pFiled)[i]));
						}
						else if (pHeader->isString())
						{
							TupleType::StringType *pFiled = (TupleType::StringType*)mTable[*it];
							listValue.push_back(TupleValue(pHeader->getName(), (*pFiled)[i]));
						}
						else if (pHeader->isInt())
						{
							TupleType::IntType *pFiled = (TupleType::IntType*)mTable[*it];
							listValue.push_back(TupleValue(pHeader->getName(), (*pFiled)[i]));
						}
					}

					pTuple->insert(listValue);
				}
			}

			mFunc = nullptr;

			return pTuple;
		}

		bool insert(vector<TupleValue> &listValue)
		{
			typedef vector<TupleValue>::iterator iterator;

			if (checkHeader(listValue) == false)
			{
				return false;
			}
			TupleType::IntType* pList = (TupleType::IntType*)mTable[_DEFAULT_];
			pList->push_back(0);

			for (iterator it = listValue.begin(); it != listValue.end(); it++)
			{
				TupleHeader	*pHeader = getHeader((*it).getName());

				if (pHeader->isDouble())
				{
					TupleType::DoubleType* pList = (TupleType::DoubleType*)mTable[(*it).getName()];
					pList->push_back((*it).getDoubleValue());
				}
				else if (pHeader->isInt())
				{
					TupleType::IntType* pList = (TupleType::IntType*)mTable[(*it).getName()];
					pList->push_back((*it).getIntValue());
				}
				else if (pHeader->isString())
				{
					TupleType::StringType* pList = (TupleType::StringType*)mTable[(*it).getName()];
					pList->push_back((*it).getStringValue());
				}
			}

			return true;
		}

		bool myDelete()
		{

			return true;
		}

		Tuple* order_by(string headerName)
		{

			return NULL;
		}

		Tuple* where(vector<string> &listHeader, WhereCondition func)
		{
			mListHeader = listHeader;
			mFunc = func;

			return this;
		}

		bool update()
		{

		}

		Tuple* top(int num)
		{
			return NULL;
		}

		Tuple* gruop_by()
		{
			return NULL;
		}

		inline int getCount()
		{
			TupleType::IntType *pList = (TupleType::IntType*)mTable[_DEFAULT_];
			if (pList == NULL)
			{
				return 0;
			}

			return pList->size();
		}

		inline bool toFirst()
		{
			mCurrentPointer = 0;

			return true;
		}

		inline bool toLast()
		{
			mCurrentPointer = mTable.size() - 1 > 0 ? mTable.size() - 1 : 0;

			return true;
		}

		inline bool toNext()
		{
			CHECK_ERROR(mCurrentPointer < mTable.size(), "越界，已经到达最后一行数据");
			mCurrentPointer++;

			return true;
		}
		inline int getCurPos()
		{
			return mCurrentPointer;
		}
		inline int begin()
		{
			return 0;
		}
		inline int end()
		{
			TupleType::IntType *pList = (TupleType::IntType*)mTable[_DEFAULT_];
			if (pList == NULL)
			{
				return 0;
			}

			return pList->size();
		}

		TupleValue operator[](string headerName)
		{
			TupleHeader* pHeader = getHeader(headerName);
			CHECK_ERROR(pHeader != NULL, "字段"+headerName+"不存在");

			if (pHeader->isDouble())
			{
				return TupleValue(headerName, (*(TupleType::DoubleType*)mTable[headerName])[mCurrentPointer]);
			}
			else if (pHeader->isInt())
			{
				return TupleValue(headerName, (*(TupleType::IntType*)mTable[headerName])[mCurrentPointer]);
			}
			else if (pHeader->isString())
			{
				return TupleValue(headerName, (*(TupleType::StringType*)mTable[headerName])[mCurrentPointer]);
			}

			CHECK_ERROR(false, "未知异常");
		}

	protected:
		bool checkHeader(vector<TupleValue> &listValue)
		{
			typedef vector<TupleValue>::iterator iterator;

			for (iterator it = listValue.begin(); it != listValue.end(); it++)
			{
				TupleHeader	*pHeader = getHeader((*it).getName());
				if (pHeader == NULL)
				{
					return false;
				}
			}

			return true;
		}
		bool checkHeader(vector<string> &listHeader)
		{
			typedef vector<string>::iterator iterator;

			for (iterator it = listHeader.begin(); it != listHeader.end(); it++)
			{
				TupleHeader	*pHeader = getHeader(*it);
				if (pHeader == NULL)
				{
					return false;
				}
			}

			return true;
		}
		TupleHeader*	getHeader(string headerName)
		{
			typedef vector<TupleHeader>::iterator iterator;

			for (iterator it = mTupleHeader.begin(); it != mTupleHeader.end(); ++it)
			{
				if ((*it).getName() == headerName)
				{
					return &(*it);
				}
			}

			return NULL;
		}
		bool addIntList(string headerName)
		{
			if (mTable[headerName] == NULL)
			{
				mTable[headerName] = new TupleType::IntType;
				mIntListGC.add((TupleType::IntType*)mTable[headerName]);

				return true;
			}

			return false;
		}
		bool addDoubleList(string headerName)
		{
			if (mTable[headerName] == NULL)
			{
				mTable[headerName] = new TupleType::DoubleType;
				mDoubleListGC.add((TupleType::DoubleType*)mTable[headerName]);

				return true;
			}

			return false;
		}
		bool addStringList(string headerName)
		{
			if (mTable[headerName] == NULL)
			{
				mTable[headerName] = new TupleType::StringType;
				mStringListGC.add((TupleType::StringType*)mTable[headerName]);
				return true;
			}

			return false;
		}

	private:
		TupleTable	mTable;
		Garbage<TupleType::DoubleType> mDoubleListGC;
		Garbage<TupleType::IntType> mIntListGC;
		Garbage<TupleType::StringType> mStringListGC;
		vector<TupleHeader>	mTupleHeader;
		WhereCondition mFunc;
		vector<string> mListHeader;

		int mCurrentPointer;
	};
}


#endif
