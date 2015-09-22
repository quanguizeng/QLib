
#ifndef QLIB_UNITTEST
#define QLIB_UNITTEST

namespace unittest
{
#define TEST_CHECK_ERROR(CONDITION,DESCRIPTION) do{if(!(CONDITION))throw DESCRIPTION;}while(0)
#define TEST_ASSERT(CONDITION) do{TEST_CHECK_ERROR(CONDITION,L"");}while(0)
#define TEST_CASE(NAME)\
		extern void TESTCASE_##NAME();								\
		namespace qlib_unittest_executors							\
		{															\
			class TESTCASE_RUNNER_##NAME							\
			{														\
			public:													\
				TESTCASE_RUNNER_##NAME()							\
				{													\
					TESTCASE_##NAME();								\
				}													\
			} TESTCASE_RUNNER_##NAME##_INSTANCE;					\
		}															\
		void TESTCASE_##NAME()
}

#endif