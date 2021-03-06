

#include "stdafx.h"
#include <crtdbg.h>
#include "../../UnitTest/GarbageTest.h"
#include "../../UnitTest/DelegateTest.h"
#include "../../UnitTest/TupleTest.h"
#include "../../UnitTest/ReflectionTest.h"
#include "../../UnitTest/MemoryStreamTest.h"
#include "../../UnitTest/FileStreamTest.h"
#include "../../UnitTest/SerializationTest.h"
#include "../../UnitTest/StreamHelperTest.h"
#include "../../UnitTest/TokenizerTest.h"

int _tmain(int argc, _TCHAR* argv[])
{
	{
		Garbage<ClassFactory> classFactoryGC(ClassFactoryInstance());
	}

	_CrtDumpMemoryLeaks();
	return 0;
}

