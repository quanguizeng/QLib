
#ifndef QLIB_FILE_STREAM_TEST
#define QLIB_FILE_STREAM_TEST

#include "UnitTest.h"
#include "../Common/Stream/FileStream.h"

TEST_CASE(testFileStream)
{
	{
		FileStream outFile(L"test.dat", FileStream::FileMode::CreateNew);

		int x = 100;
		int y = 200;
		int z = 300;
		int result;

		result = outFile.write(&x, sizeof(int));
		assert(result == sizeof(int));
		result =  outFile.write(&y, sizeof(int));
		assert(result == sizeof(int));
		result = outFile.write(&z, sizeof(int));
		assert(result == sizeof(int));

		outFile.flush();
		outFile.close();
	}

	{
		FileStream inFile(L"test.dat", FileStream::FileMode::ReadOnly);

		int x;
		int y;
		int z;
		int result;

		result = inFile.read(&x, sizeof(int));
		assert(inFile.isEnd() == false);
		assert(result == sizeof(int));
		assert(x == 100);
		result = inFile.read(&y, sizeof(int));
		assert(result == sizeof(int));
		assert(y == 200);
		result = inFile.read(&z, sizeof(int));
		assert(result == sizeof(int));
		assert(z == 300);
		assert(inFile.isEnd() == true);

		inFile.close();
	}
}

TEST_CASE(testFileStreamReadWriteMode)
{

	{
		FileStream fileStream(L"test.dat", FileStream::FileMode::ReadWrite);

		int x;
		int y;
		int z;
		int result;

		result = fileStream.read(&x, sizeof(int));
		assert(result == sizeof(int));
		assert(x == 100);
		assert(fileStream.isEnd() == false);

		x = 400;
		result = fileStream.write(&x, sizeof(int));
		assert(result == sizeof(int));
		assert(fileStream.isEnd() == false);

		result = fileStream.read(&y, sizeof(int));
		assert(result == sizeof(int));
		assert(y == 300);

		assert(fileStream.isEnd() == true);

		fileStream.close();
	}

	{
		FileStream fileStream(L"test.dat", FileStream::FileMode::ReadWrite);

		int x;
		int y;
		int z;
		int result;

		result = fileStream.read(&x, sizeof(int));
		assert(result == sizeof(int));
		assert(x == 100);
		assert(fileStream.isEnd() == false);
		result = fileStream.read(&y, sizeof(int));
		assert(result == sizeof(int));
		assert(y == 400);
		result = fileStream.read(&z, sizeof(int));
		assert(result == sizeof(int));
		assert(z == 300);

		assert(fileStream.isEnd() == true);

		fileStream.close();
	}
}

#endif