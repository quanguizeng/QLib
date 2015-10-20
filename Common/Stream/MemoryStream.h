//##########################################################################
//Qlib 1.0
//开发者:曾全贵
//##########################################################################


#ifndef QLIB_MEMERY_STREAM
#define QLIB_MEMERY_STREAM

#include "Interfaces.h"

namespace QLib
{
	class MemoryStream :public virtual IStream
	{
		enum MemoryMode
		{
			ReadOnly,
			ReadWrite
		};
	public:
		MemoryStream(int max = 4096)
		{
			CHECK_ERROR(max > 0, L"长度不能为负数");

			mMode = MemoryMode::ReadWrite;

			if (max <= 0)
			{
				mMaxSize = 0;
				mBuffer = nullptr;
				mStreamSize = 0;
				mCurPos = 0;

				return;
			}

			if (max % 512 != 0)
			{
				max = ((max + 1) / 512 + 1) * 512;
			}

			mMaxSize = max;
			mBuffer = new char[mMaxSize];
			mStreamSize = 0;
			mCurPos = 0;
		}
		MemoryStream(void *pBuffer, int len)
		{
			CHECK_ERROR(pBuffer != nullptr && len > 0, L"指针不能为空或者长度不能为负数");

			mMode = MemoryMode::ReadOnly;

			mMaxSize = len;
			mBuffer = (char*)pBuffer;
			mStreamSize = len;
			mCurPos = 0;
		}
		~MemoryStream()
		{
			close();
		}

	public:
		inline int size()
		{
			return mStreamSize;
		}
		inline bool close()
		{
			if (mMode == MemoryMode::ReadWrite)
			{
				if (mBuffer != nullptr)
				{
					delete mBuffer;
				}
				mBuffer = nullptr;
				mStreamSize = 0;
				mCurPos = 0;
			}
			else if (mMode == MemoryMode::ReadOnly)
			{
				mMaxSize = 0;
				mBuffer = nullptr;
				mStreamSize = 0;
				mCurPos = 0;
			}

			return true;
		}
		inline bool isEnd()
		{
			return mStreamSize == 0;
		}
		int	read(void* pBuffer, int nSize)
		{
			if (mMode == MemoryMode::ReadOnly)
			{
				readReadOnlyMode(pBuffer, nSize);
			}
			else if (mMode == MemoryMode::ReadWrite)
			{
				readReadWriteMode(pBuffer, nSize);
			}

			return nSize;
		}
		int	write(void* pBuffer, int nSize)
		{
			CHECK_ERROR(mMode == MemoryMode::ReadWrite, L"这是只读内存流，不能往流里写入数据");
			CHECK_ERROR(pBuffer != nullptr && nSize >= 0, L"指针不能为空或者长度不能为负数");

			int lastMaxSize = mMaxSize;
			mMaxSize = nSize + mStreamSize >= mMaxSize ? ((nSize + mStreamSize + 1) / 512 + 1) * 512 : mMaxSize;
			if (mMaxSize > lastMaxSize)
			{
				char *pNewBuffer = new char[mMaxSize];
				memcpy(pNewBuffer, mBuffer, mStreamSize);
				memcpy(pNewBuffer + mStreamSize, pBuffer, nSize);
				mStreamSize += nSize;

				delete mBuffer;
				mBuffer = pNewBuffer;
			}
			else
			{
				memcpy(mBuffer + mStreamSize, pBuffer, nSize);
				mStreamSize += nSize;
			}

			return nSize;
		}

		inline void* getBuffer()
		{
			return mBuffer;
		}

	private:
		inline int	readReadOnlyMode(void* pBuffer, int nSize)
		{
			CHECK_ERROR(pBuffer != nullptr && nSize > 0, L"指针不能为空或者长度不能为负数");
			nSize = nSize > mStreamSize ? mStreamSize : nSize;

			memcpy(pBuffer, mBuffer + mCurPos, nSize);
			mCurPos += nSize;
			mStreamSize -= nSize;

			return nSize;
		}
		inline int	readReadWriteMode(void* pBuffer, int nSize)
		{
			CHECK_ERROR(pBuffer != nullptr && nSize > 0, L"指针不能为空或者长度不能为负数");

			nSize = nSize > mStreamSize ? mStreamSize : nSize;

			memcpy(pBuffer, mBuffer + mCurPos, nSize);
			mCurPos += nSize;
			mStreamSize -= nSize;

			memcpy(mBuffer, mBuffer + mCurPos, mStreamSize);
			mCurPos = 0;

			return nSize;
		}

	protected:
		char *mBuffer;
		int mMaxSize;
		int mStreamSize;
		int mCurPos;
		MemoryMode mMode;
	};
}
#endif