//##########################################################################
//Qlib 1.0
//开发者:曾全贵
//##########################################################################


#ifndef QLIB_SERIALIZATION
#define QLIB_SERIALIZATION

#include "Interfaces.h"
#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>
#include "../Reflection.h"
#include <functional>

using namespace std;

namespace QLib
{
	template<typename T>
	class Serialization
	{
	public:
		static bool write(IStream &stream,  const T &value)
		{
			return stream.write((void*)&value, sizeof(T)) > 0;
		}
		static bool read(IStream &stream, T &value)
		{
			return stream.read((void*)&value, sizeof(T)) > 0;
		}
	};

	template<typename T>
	bool writeStream(IStream &stream, const T &value)
	{
		return Serialization<T>::write(stream, value);
	};

	template<typename T>
	T readStream(IStream &stream)
	{
		T value;
		Serialization<T>::read(stream, value);

		return value;
	}

	template<>
	class Serialization<string>
	{
	public:
		static bool write(IStream &stream, const string &value)
		{
			int len = value.size();
			writeStream<int>(stream, len);

			stream.write((void*)value.c_str(), value.size());

			return true;
		}
		static bool read(IStream &stream, string &value)
		{
			int len = readStream<int>(stream);
			Garbage<char> gc;

			char *pStr = new char[len + 1];
			gc.add(pStr);

			stream.read(pStr, len);
			pStr[len] = 0;

			value = string(pStr);

			return true;
		}
	};

	class ClassSerializationBase;

	template<typename TUnit, typename TContainer>
	class ContainerSerialization
	{
	public:
		static bool write(IStream &stream, const TContainer &list)
		{
			typedef TContainer::const_iterator iterator;

			int len = list.size();
			writeStream<int>(stream, len);

			if (list.size() > 0)
			{
				for (iterator it = list.begin(); it != list.end(); it++)
				{
					writeStream<TUnit>(stream, *it);
				}
			}

			return true;
		}
		static bool read(IStream &stream, TContainer &list)
		{
			typedef TContainer::const_iterator iterator;

			int len = readStream<int>(stream);

			for (int i = 0; i < len; i++)
			{
				TUnit value;
				value = readStream<TUnit>(stream);
				list.push_back(value);
			}

			return true;
		}
	};

	template<typename TUnit>
	class ContainerSerialization<TUnit, set<TUnit>>
	{
	public:
		static bool write(IStream &stream, const set<TUnit> &list)
		{
			typedef set<TUnit>::const_iterator iterator;

			int len = list.size();
			writeStream<int>(stream, len);

			if (list.size() > 0)
			{
				for (iterator it = list.begin(); it != list.end(); it++)
				{
					writeStream<TUnit>(stream, *it);
				}
			}

			return true;
		}
		static bool read(IStream &stream, set<TUnit> &list)
		{
			typedef set<TUnit>::const_iterator iterator;

			int len = readStream<int>(stream);

			for (int i = 0; i < len; i++)
			{
				TUnit value;
				value = readStream<TUnit>(stream);
				list.insert(value);
			}

			return true;
		}
	};

	template<typename TKey, typename TValue, typename TContainer>
	class MapSerialization
	{
	public:
		static bool write(IStream &stream, const TContainer &list)
		{
			typedef TContainer::const_iterator iterator;

			int len = list.size();
			writeStream<int>(stream, len);

			if (list.size() > 0)
			{
				for (iterator it = list.begin(); it != list.end(); it++)
				{
					writeStream<TKey>(stream, it->first);
					writeStream<TValue>(stream, it->second);
				}
			}

			return true;
		}
		static bool read(IStream &stream, TContainer &list)
		{
			typedef TContainer::const_iterator iterator;

			int len = readStream<int>(stream);

			for (int i = 0; i < len; i++)
			{
				TKey key;
				key = readStream<TKey>(stream);

				TValue value;
				value = readStream<TValue>(stream);

				list.insert(pair<TKey, TValue>(key, value));
			}

			return true;
		}
	};

	class ClassSerializationBase
	{
	public:
		virtual bool serialization(IStream &stream) = 0;
		virtual bool deserialization(IStream &stream) = 0;
	};

	template<typename T>
	class ClassSerialization :public ClassSerializationBase
	{
	public:
		ClassSerialization(T *pObject)
		{
			mObject = pObject;
		}
		virtual ~ClassSerialization()
		{
		}

	public:
		virtual bool serialization(IStream &stream)
		{
			MemberBase *pObject = (MemberBase*)mObject;

			return true;
		}

		virtual bool deserialization(IStream &stream)
		{
			MemberBase *pObject = (MemberBase*)mObject;

			return true;
		}

	protected:
		T *mObject;
	};

}

#endif