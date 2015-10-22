
#ifndef QLIB_STREAM_HELPER_TEST
#define QLIB_STREAM_HELPER_TEST

#include "UnitTest.h"
#include "../Common/Stream/StreamHelper.h"

TEST_CASE(testStreamHelper)
{
	{
		FileStream fileStream1(L"test.txt", FileStream::FileMode::CreateNew);
		StringWriter stringWriter(&fileStream1);

		stringWriter.writeLine("AAAAA");
		stringWriter.writeLine("BBBBB");
		stringWriter.writeLine("CCCCC");
		stringWriter.write('D');
		
		fileStream1.close();
		stringWriter.close();

		FileStream fileStream2(L"test.txt", FileStream::FileMode::ReadOnly);
		StringReader stringReader(&fileStream2);

		string str = stringReader.readLine();
		assert(str == "AAAAA");
		str = stringReader.readLine();
		str = stringReader.readLine();
		assert(str == "BBBBB");
		str = stringReader.readLine();
		str = stringReader.readLine();
		assert(str == "CCCCC");
		str = stringReader.readLine();
		char c = stringReader.read();
		assert(c == 'D');
		assert(fileStream2.isEnd() == true);

		fileStream2.close();
	}
}

#endif