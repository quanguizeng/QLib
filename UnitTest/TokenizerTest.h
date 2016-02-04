
#ifndef QLIB_TOKENIZER_TEST
#define QLIB_TOKENIZER_TEST

#include "../common/Tokenizer.h"
#include "UnitTest.h"
#include<string>
#include<list>
#include<assert.h>
#include<vector>
#include <map>


using namespace std;

using namespace QLib;

TEST_CASE(testTokenizer)
{
	{
		string str = ";!!;Hello|world||-foo--bar;yow;baz|";
		CharSeperator<char> seperator("-;|");
		Tokenizer<CharSeperator<char>> tokenizer(str, seperator);

		auto strBuffer = tokenizer.begin();

		assert(tokenizer.getSize() == 12);

		assert(**strBuffer++ == "");
		assert(**(strBuffer++) == "!!");
		assert(**(strBuffer++) == "Hello");
		assert(**(strBuffer++) == "world");
		assert(**(strBuffer++) == "");
		assert(**(strBuffer++) == "");
		assert(**(strBuffer++) == "foo");
		assert(**(strBuffer++) == "");
		assert(**(strBuffer++) == "bar");
		assert(**(strBuffer++) == "yow");
		assert(**(strBuffer++) == "baz");
		assert(**(strBuffer++) == "");
	}
	{
		wstring str = L";!!;Hello|world||-foo--bar;yow;baz|";
		CharSeperator<wchar_t> seperator(L"-;|");
		Tokenizer<CharSeperator<wchar_t>> tokenizer(str, seperator);

		auto strBuffer = tokenizer.begin();

		assert(tokenizer.getSize() == 12);

		assert(**strBuffer++ == L"");
		assert(**(strBuffer++) == L"!!");
		assert(**(strBuffer++) == L"Hello");
		assert(**(strBuffer++) == L"world");
		assert(**(strBuffer++) == L"");
		assert(**(strBuffer++) == L"");
		assert(**(strBuffer++) == L"foo");
		assert(**(strBuffer++) == L"");
		assert(**(strBuffer++) == L"bar");
		assert(**(strBuffer++) == L"yow");
		assert(**(strBuffer++) == L"baz");
		assert(**(strBuffer++) == L"");
	}
}

#endif