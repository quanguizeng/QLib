
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
	public:
		enum eType;

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
			mStringValue = "";
			mIntValue = 0;
			mDoubleValue = 0;
		}
		TupleValue(string headerName, string value)
		{
			mName = headerName;
			mStringValue = value;
			mIntValue = 0;
			mDoubleValue = 0;
		}
		TupleValue(string headerName, int value)
		{
			mName = headerName;
			mStringValue = "";
			mIntValue = value;
			mDoubleValue = 0;
		}
		TupleValue(string headerName, double value)
		{
			mName = headerName;
			mStringValue = "";
			mIntValue = 0;
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
	class TupleRow
	{
	public:
		typedef map<string, TupleValue>		ListTupleValue;
		TupleRow()
		{

		}
		TupleRow(ListTupleValue &listValue)
		{
			mListValue = listValue;
		}
		~TupleRow()
		{
			mListValue.clear();
		}

		inline TupleValue operator[](string name)
		{
			CHECK_ERROR(mListValue.find(name) != mListValue.end(), "查询的字段名不存在!");


			return mListValue[name];
		}

		inline ListTupleValue &getList()
		{
			return mListValue;
		}

	protected:
		ListTupleValue mListValue;
	};
	class Tuple;

	template<typename T>
	class OrderBy
	{
	public:
		OrderBy()
		{

		}
		~OrderBy()
		{
		}

		static void insertSortAsc(string sortName, Tuple *pTuple)
		{
			TupleHeader *pHeader = pTuple->getHeader(sortName);
			CHECK_ERROR(pHeader != NULL, "排序字段不存在!");
			CHECK_ERROR(pTuple != NULL, "");

			vector<T> *pListColumn = (vector<T>*)pTuple->getTable()[sortName];

			for (int i = 1; i < pListColumn->size(); i++)
			{
				if ((*pListColumn)[i - 1] >(*pListColumn)[i])
				{
					T temp = (*pListColumn)[i];
					TupleRow rowTemp = (*pTuple)[i];
					int j = i;
					while (j > 0 && (*pListColumn)[j - 1] > temp)
					{
						pTuple->setRow(j, (*pTuple)[j - 1]);
						j--;
					}
					pTuple->setRow(j, rowTemp);
				}
			}
		};

		static void insertSortDesc(string sortName, Tuple *pTuple)
		{
			TupleHeader *pHeader = pTuple->getHeader(sortName);
			CHECK_ERROR(pHeader != NULL, "排序字段不存在!");
			CHECK_ERROR(pTuple != NULL, "");

			vector<T> *pListColumn = (vector<T>*)pTuple->getTable()[sortName];

			for (int i = 1; i < pListColumn->size(); i++)
			{
				if ((*pListColumn)[i - 1] < (*pListColumn)[i])
				{
					T temp = (*pListColumn)[i];
					TupleRow rowTemp = (*pTuple)[i];
					int j = i;
					while (j > 0 && (*pListColumn)[j - 1] < temp)
					{
						pTuple->setRow(j, (*pTuple)[j - 1]);
						j--;
					}
					pTuple->setRow(j, rowTemp);
				}
			}
		};

	};

	template<typename T>
	class GruopBy
	{
	public:
		GruopBy()
		{
			
		}
		~GruopBy()
		{

		}

		static void insertSort(string sortName, Tuple *pTuple)
		{
			TupleHeader *pHeader = pTuple->getHeader(sortName);
			CHECK_ERROR(pHeader != NULL, "排序字段不存在!");
			CHECK_ERROR(pTuple != NULL, "");

			vector<T> *pListColumn = (vector<T>*)pTuple->getTable()[sortName];

			for (int i = 1; i < pListColumn->size(); i++)
			{
				if ((*pListColumn)[i - 1] >(*pListColumn)[i])
				{
					T temp = (*pListColumn)[i];
					TupleRow rowTemp = (*pTuple)[i];
					int j = i;
					while (j > 0 && (*pListColumn)[j - 1] > temp)
					{
						pTuple->setRow(j, (*pTuple)[j - 1]);
						j--;
					}
					pTuple->setRow(j, rowTemp);
				}
			}
		}
	};


	class Tuple
	{
		friend class OrderBy<double>;
		friend class OrderBy<int>;
		friend class OrderBy<string>;
		friend class GruopBy<double>;
		friend class GruopBy<int>;
		friend class GruopBy<string>;
	public:
		typedef vector<TupleHeader> ListTupleHeader;
		typedef	TupleHeader::eType	HeaderType;
		typedef map<string, void*> TupleTable;
		typedef function< bool(map<string, TupleValue>&)>	WhereCondition;
		typedef function<bool(map<string, vector<TupleValue>>&)> OrderCondition;

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
				if (mFunc == nullptr || mFunc(mapValue))
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

		bool my_delete()
		{
			typedef vector<string>::iterator iterator;

			for (int i = 0; i < getCount(); i++)
			{
				map<string, TupleValue> mapValue;

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

				if (mFunc == nullptr || mFunc(mapValue))
				{
					typedef vector<TupleHeader>::iterator iterator;

					for (iterator it = mTupleHeader.begin(); it != mTupleHeader.end(); ++it)
					{
						TupleHeader &header = *it;
						if (header.isDouble())
						{
							TupleType::DoubleType *pFiled = (TupleType::DoubleType*)mTable[(*it).getName()];
							TupleType::DoubleType::iterator first = pFiled->begin() + i;
							pFiled->erase(first);
						}
						else if (header.isString())
						{
							TupleType::StringType *pFiled = (TupleType::StringType*)mTable[(*it).getName()];
							TupleType::StringType::iterator first = pFiled->begin() + i;
							pFiled->erase(first);
						}
						else if (header.isInt())
						{
							TupleType::IntType *pFiled = (TupleType::IntType*)mTable[(*it).getName()];
							TupleType::IntType::iterator first = pFiled->begin() + i;
							pFiled->erase(first);
						}
					}
					i--;
				}
			}

			mFunc = nullptr;

			return true;
		}

		Tuple* order_by(string headerName, string flag)
		{
			TupleHeader *pHeader = getHeader(headerName);
			CHECK_ERROR(pHeader != NULL, "排序字段不存在!");

			if (flag == "asc")
			{
				// 升序排序
				if (pHeader->isDouble())
				{
					OrderBy<double>::insertSortAsc(headerName, this);
				}
				else if (pHeader->isInt())
				{
					OrderBy<int>::insertSortAsc(headerName, this);
				}
				else if (pHeader->isString())
				{
					OrderBy<string>::insertSortAsc(headerName, this);
				}
			}
			else if (flag == "" || flag == "desc")
			{
				// 默认降序排序
				if (pHeader->isDouble())
				{
					OrderBy<double>::insertSortDesc(headerName, this);
				}
				else if (pHeader->isInt())
				{
					OrderBy<int>::insertSortDesc(headerName, this);
				}
				else if (pHeader->isString())
				{
					OrderBy<string>::insertSortDesc(headerName, this);
				}
			}
			return this;
		}

		Tuple* where(vector<string> &listHeader, WhereCondition func)
		{
			mListHeader = listHeader;
			mFunc = func;

			return this;
		}

		bool update(vector<TupleValue> &listTupleValue)
		{
			CHECK_ERROR(listTupleValue.size() > 0, "需要指明更新字段!");
			CHECK_ERROR(checkHeader(listTupleValue) == true, "存在未知字段!");

			typedef vector<string>::iterator iterator;

			for (int i = 0; i < getCount(); i++)
			{
				map<string, TupleValue> mapValue;
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
				if (mFunc == nullptr || mFunc(mapValue))
				{
					for (vector<TupleValue>::iterator it = listTupleValue.begin(); it != listTupleValue.end(); it++)
					{
						TupleHeader *pHeader = getHeader((*it).getName());
						if (pHeader->isDouble())
						{
							TupleType::DoubleType *pFiled = (TupleType::DoubleType*)mTable[pHeader->getName()];
							(*pFiled)[i] = (*it).getDoubleValue();
						}
						else if (pHeader->isString())
						{
							TupleType::StringType *pFiled = (TupleType::StringType*)mTable[pHeader->getName()];
							(*pFiled)[i] = (*it).getStringValue();
						}
						else if (pHeader->isInt())
						{
							TupleType::IntType *pFiled = (TupleType::IntType*)mTable[pHeader->getName()];
							(*pFiled)[i] = (*it).getIntValue();
						}
					}
				}
			}

			mFunc = nullptr;

			return true;
		}

		Tuple* top(int num)
		{
			CHECK_ERROR(num > 0, "指定获取的数目不对!");

			if (num >= getCount())
			{
				return this;
			}

			for (ListTupleHeader::iterator it = mTupleHeader.begin(); it != mTupleHeader.end(); it++)
			{
				TupleHeader *pHeader = &(*it);
				if (pHeader->isDouble())
				{
					TupleType::DoubleType *pFiled = (TupleType::DoubleType*)mTable[pHeader->getName()];
					TupleType::DoubleType::iterator first = pFiled->begin() + num;
					TupleType::DoubleType::iterator last = pFiled->end();
					if (first <= last)
					{
						pFiled->erase(first, last);
					}
				}
				else if (pHeader->isString())
				{
					TupleType::StringType *pFiled = (TupleType::StringType*)mTable[pHeader->getName()];
					TupleType::StringType::iterator first = pFiled->begin() + num;
					TupleType::StringType::iterator last = pFiled->end();
					if (first <= last)
					{
						pFiled->erase(first, last);
					}
				}
				else if (pHeader->isInt())
				{
					TupleType::IntType *pFiled = (TupleType::IntType*)mTable[pHeader->getName()];
					TupleType::IntType::iterator first = pFiled->begin() + num;
					TupleType::IntType::iterator last = pFiled->end();
					if (first <= last)
					{
						pFiled->erase(first, last);
					}
				}
			}

			return this;
		}

		Tuple* gruop_by(string headerName)
		{
			TupleHeader *pHeader = getHeader(headerName);
			CHECK_ERROR(pHeader != NULL, "分组字段不存在!");

			if (pHeader->isDouble())
			{
				GruopBy<double>::insertSort(headerName, this);
			}
			else if (pHeader->isInt())
			{
				GruopBy<int>::insertSort(headerName, this);
			}
			else if (pHeader->isString())
			{
				GruopBy<string>::insertSort(headerName, this);
			}

			return this;
		}

		inline int getCount()
		{
			if (mTupleHeader.size() > 0)
			{
				TupleHeader *pHeader = &mTupleHeader[0];
				if (pHeader->isDouble())
				{
					TupleType::DoubleType *pFiled = (TupleType::DoubleType*)mTable[pHeader->getName()];
					return pFiled->size();
				}
				else if (pHeader->isString())
				{
					TupleType::StringType *pFiled = (TupleType::StringType*)mTable[pHeader->getName()];
					return pFiled->size();
				}
				else if (pHeader->isInt())
				{
					TupleType::IntType *pFiled = (TupleType::IntType*)mTable[pHeader->getName()];
					return pFiled->size();
				}
			}

			return 0;
		}

		inline bool toFirst()
		{
			mCurrentPointer = 0;

			return true;
		}

		inline bool toLast()
		{
			mCurrentPointer = end();

			return true;
		}

		inline bool toNext()
		{
			CHECK_ERROR(mCurrentPointer < getCount(), "越界，已经到达最后一行数据");
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
			if (getCount() > 0)
			{
				return getCount() - 1;
			}

			return 0;
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

		TupleRow operator[](int i)
		{
			CHECK_ERROR(i >= 0 && i < getCount(), "查询下标越界!");

			TupleRow tupleRow;
			TupleRow::ListTupleValue &listValue = tupleRow.getList();

			for (ListTupleHeader::iterator it = mTupleHeader.begin(); it < mTupleHeader.end(); it++)
			{
				TupleHeader *pHeader = &(*it);
				if (pHeader->isDouble())
				{
					TupleType::DoubleType *pFiled = (TupleType::DoubleType*)mTable[pHeader->getName()];
					listValue.insert(pair<string, TupleValue>(pHeader->getName(), TupleValue(pHeader->getName(), (*pFiled)[i])));
				}
				else if (pHeader->isString())
				{
					TupleType::StringType *pFiled = (TupleType::StringType*)mTable[pHeader->getName()];
					listValue.insert(pair<string, TupleValue>(pHeader->getName(), TupleValue(pHeader->getName(), (*pFiled)[i])));
				}
				else if (pHeader->isInt())
				{
					TupleType::IntType *pFiled = (TupleType::IntType*)mTable[pHeader->getName()];
					listValue.insert(pair<string, TupleValue>(pHeader->getName(), TupleValue(pHeader->getName(), (*pFiled)[i])));
				}
			}

			return tupleRow;
		}

	protected:
		inline TupleTable& getTable()
		{
			return mTable;
		}
		bool setRow(int i, TupleRow &row)
		{
			CHECK_ERROR(i >= 0 && i < getCount(), "查询下标越界!");
			TupleRow::ListTupleValue &listValue = row.getList();

			for (ListTupleHeader::iterator it = mTupleHeader.begin(); it < mTupleHeader.end(); it++)
			{
				TupleHeader *pHeader = &(*it);

				if (pHeader->isDouble())
				{
					TupleType::DoubleType *pFiled = (TupleType::DoubleType*)mTable[pHeader->getName()];
					(*pFiled)[i] = listValue[pHeader->getName()].getDoubleValue();
				}
				else if (pHeader->isString())
				{
					TupleType::StringType *pFiled = (TupleType::StringType*)mTable[pHeader->getName()];
					(*pFiled)[i] = listValue[pHeader->getName()].getStringValue();
				}
				else if (pHeader->isInt())
				{
					TupleType::IntType *pFiled = (TupleType::IntType*)mTable[pHeader->getName()];
					(*pFiled)[i] = listValue[pHeader->getName()].getIntValue();
				}
			}

			return true;
		}
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
		ListTupleHeader	mTupleHeader;
		WhereCondition mFunc;
		vector<string> mListHeader;

		int mCurrentPointer;
	};
}


#endif
