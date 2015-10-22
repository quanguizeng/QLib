//##########################################################################
//Qlib 1.0
//开发者:曾全贵
//##########################################################################


#ifndef QLIB_STREAM_HELPER
#define QLIB_STREAM_HELPER

#include "Interfaces.h"
#include <string>

using namespace std;

namespace QLib
{
	class StringReader
	{
	public:
		StringReader(FileStream *pStream)
		{
			mStream = pStream;
		}
		~StringReader()
		{
			mStream = nullptr;
		}

	public:
		bool close()
		{
			mStream = nullptr;

			return true;
		}
		inline char read()
		{
			CHECK_ERROR(mStream != nullptr, L"流指针为空");

			char c;
			mStream->read(&c, sizeof(char));

			return c;
		}
		string readLine()
		{
			CHECK_ERROR(mStream != nullptr, L"流指针不能为空");

			string result = "";
			char c;

			while (mStream->isEnd() == false)
			{
				mStream->read(&c, sizeof(char));
				if (c == '\r' || c == '\n')
				{
					break;
				}
				result += c;
			}

			return result;
		}
		string readToEnd()
		{
			CHECK_ERROR(mStream != nullptr, L"流指针不能为空");

			int len = mStream->size();
			char *pStr = new char[len + 1];
			Garbage<char> gc;

			gc.add(pStr);
			mStream->read(pStr, len);
			pStr[len] = 0;

			return string(pStr);
		}

	protected:
		FileStream *mStream;
	};

	class StringWriter
	{
	public:
		StringWriter(FileStream *pStream)
		{
			mStream = pStream;
		}

		~StringWriter()
		{
			mStream = nullptr;
		}

	public:
		bool close()
		{
			mStream = nullptr;

			return true;
		}

		bool flush()
		{
			CHECK_ERROR(mStream != nullptr, L"流指针不能为空");

			mStream->flush();
		}

		bool write(char c)
		{
			CHECK_ERROR(mStream != nullptr, L"流指针不能为空");

			return mStream->write(&c, sizeof(char)) > 0;
		}
		bool writeLine(string str)
		{
			CHECK_ERROR(mStream != nullptr, L"流指针不能为空");
			char c = '\r';

			int len = mStream->write((void*)str.c_str(), str.size());
			len += mStream->write(&c, sizeof(char));
			c = '\n';
			len += mStream->write(&c, sizeof(char));
			return len == (str.size() + sizeof(char)*2);
		}

	protected:
		FileStream *mStream;

	};
}

#endif