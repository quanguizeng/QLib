
#ifndef QLIB_MEMORY_STREAM_TEST
#define QLIB_MEMORY_STREAM_TEST

#include "UnitTest.h"
#include "../Common/Stream/MemoryStream.h"

TEST_CASE(testMemoryStreamReadWriteMode)
{
	{
		MemoryStream memStream;
		{
			int x = 100;
			int y = 200;
			char name[] = "AAAA";

			memStream.write((void*)&x, sizeof(x));
			assert(memStream.size() == sizeof(x));
			memStream.write((void*)&y, sizeof(y));
			assert(memStream.size() == sizeof(x)*2);
			memStream.write((void*)name, sizeof(name));
			assert(memStream.size() == sizeof(x) * 2 + sizeof(name));
		}
		{
			int x;
			int y;
			char name[16];
			char name1[] = "AAAA";
			memStream.read((void*)&x, sizeof(x));
			assert(x == 100);
			assert(memStream.isEnd() == false);
			assert(memStream.size() == sizeof(x) + sizeof(name1));
			memStream.read((void*)&y, sizeof(y));
			assert(y == 200);
			memStream.read((void*)name, sizeof(name));
			assert(string(name) == "AAAA");
			assert(memStream.isEnd() == true);
		}

		memStream.close();
	}

	{
		MemoryStream memStream;
		{
			int x = 100;
			int y = 200;
			char name[] = "AAAA";
			char name1[32] = "";

			memStream.write((void*)&x, sizeof(x));
			assert(memStream.size() == sizeof(x));
			memStream.write((void*)&y, sizeof(y));
			assert(memStream.size() == sizeof(x) * 2);
			memStream.write((void*)name, sizeof(name));
			assert(memStream.size() == sizeof(x) * 2 + sizeof(name));

			x = 0;
			memStream.read((void*)&x, sizeof(x));
			assert(x == 100);
			assert(memStream.size() == sizeof(x) + sizeof(name));
			x = 300;
			memStream.write((void*)&x, sizeof(x));
			assert(memStream.size() == sizeof(x) * 2 + sizeof(name));

			memStream.read((void*)&y, sizeof(y));
			assert(y == 200);

			memStream.read((void*)name1, sizeof(name));
			assert(string(name1) == "AAAA");
			assert(memStream.isEnd() == false);

			memStream.read((void*)&y, sizeof(y));
			assert(y == 300);
			assert(memStream.isEnd() == true);
			assert(memStream.size() == 0);
		}

		memStream.close();
	}
}

TEST_CASE(testMemoryStreamReadOnlyMode)
{
	{
		MemoryStream memStreamReadWriteMode;
		{
			int x = 100;
			int y = 200;
			char name[] = "AAAA";

			memStreamReadWriteMode.write((void*)&x, sizeof(x));
			assert(memStreamReadWriteMode.size() == sizeof(x));
			memStreamReadWriteMode.write((void*)&y, sizeof(y));
			assert(memStreamReadWriteMode.size() == sizeof(x) * 2);
			memStreamReadWriteMode.write((void*)name, sizeof(name));
			assert(memStreamReadWriteMode.size() == sizeof(x) * 2 + sizeof(name));
		}

		MemoryStream memStreamReadOnlyMode(memStreamReadWriteMode.getBuffer(), memStreamReadWriteMode.size());
		{
			int x;
			int y;
			char name[16];
			char name1[] = "AAAA";
			memStreamReadOnlyMode.read((void*)&x, sizeof(x));
			assert(x == 100);
			assert(memStreamReadOnlyMode.isEnd() == false);
			assert(memStreamReadOnlyMode.size() == sizeof(x) + sizeof(name1));
			memStreamReadOnlyMode.read((void*)&y, sizeof(y));
			assert(y == 200);
			memStreamReadOnlyMode.read((void*)name, sizeof(name));
			assert(string(name) == "AAAA");
			assert(memStreamReadOnlyMode.isEnd() == true);
		}

		memStreamReadWriteMode.close();
		memStreamReadOnlyMode.close();
	}

}

#endif