//##########################################################################
//Qlib 1.0
//开发者:曾全贵
//##########################################################################

#ifndef QLIB_FILESTREAM
#define QLIB_FILESTREAM

#include "Interfaces.h"
#include <stdio.h>
#include <string>
#include <io.h>

using namespace std;

namespace QLib
{
	class FileStream :public virtual IStream
	{
	public:
		enum FileMode
		{
			ReadOnly,
			ReadWrite,
			CreateNew
		};
	public:
		FileStream(wstring filePath, FileMode fileMode)
		{
			wchar_t s;
			wstring mode;

			mFile = nullptr;

			switch (fileMode)
			{
			case FileMode::ReadOnly:
			{
				mode = L"rb";

				break;
			}
			case FileMode::ReadWrite:
			{
				mode = L"r+b";

				break;
			}
			case FileMode::CreateNew:
			{
				mode = L"w+b";

				break;
			}
			}

			if (_wfopen_s(&mFile, filePath.c_str(), mode.c_str()) != 0)
			{
				CHECK_ERROR(false, L"无法打开文件");
			}
		}
		~FileStream()
		{
			close();
		}

		int size()
		{
			CHECK_ERROR(mFile != nullptr, L"请先打开文件");

			fpos_t curPos;
			if(fgetpos(mFile, &curPos) == 0)
			{
				if (fseek(mFile, 0, SEEK_END) == 0)
				{
					fpos_t fileSize = 0;
					if (fgetpos(mFile, &fileSize) == 0)
					{
						if (fsetpos(mFile, &curPos) == 0)
						{
							return fileSize;
						}
					}
				}
			}

			return -1;
		}
		bool isEnd()
		{
			CHECK_ERROR(mFile != nullptr, L"请先打开文件");

			fpos_t curPos;
			if (fgetpos(mFile, &curPos) == 0)
			{
				if (fseek(mFile, 0, SEEK_END) == 0)
				{
					fpos_t fileSize = 0;
					if (fgetpos(mFile, &fileSize) == 0)
					{
						if (fsetpos(mFile, &curPos) == 0)
						{
							return curPos == fileSize;
						}
					}
				}
			}

			return true;
		}
		inline bool close()
		{
			if (mFile != nullptr)
			{
				fclose(mFile);
				mFile = nullptr;

				return true;
			}

			return false;
		}
		inline int	read(void* pBuffer, int nSize)
		{
			CHECK_ERROR(mFile != nullptr, L"请先打开文件");
			CHECK_ERROR(pBuffer != nullptr && nSize >= 0, L"指针为NULL或者大小为负数");
			CHECK_ERROR(nSize <= size(), L"数据不足，无法读取指定大小数据");

			return fread(pBuffer, 1, nSize, mFile);
		}
		inline int	write(void* pBuffer, int nSize)
		{
			CHECK_ERROR(mFile != nullptr, L"请先打开文件");
			CHECK_ERROR(pBuffer != nullptr && nSize >= 0, L"指针为NULL或者大小为负数");

			return fwrite(pBuffer, 1, nSize, mFile);
		}
		inline bool flush()
		{
			return fflush(mFile) > 0;
		}

	protected:
		FileMode mFileMode;
		FILE*	mFile;
	};
}


#endif