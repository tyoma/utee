#include <ut/tee.h>

#include <ut/assert.h>
#include <ut/test.h>

namespace ut
{
	namespace tests
	{
		class TestClass1
		{
		public:
			void foo()
			{
			}

			void bar()
			{
			}
		};


		class TestClass2
		{
		public:
			void foo()
			{
			}
		};


		class TestClass3
		{
		public:
			void foo()
			{
			}
		};


		begin_test_suite(TestExecutionEngineTests)
			test(TeeDoesNotHaveSuitesByDefault)
			{
				// INIT
				tee t;
				const tee &const_t(t);

				// ACT / ASSERT
				are_equal(0, const_t.suites_count());
			}


			test(RegisteringTestChangesSuitesCount)
			{
				// INIT
				tee t;

				// ACT
				t.add_test(&TestClass1::foo);

				// ASSERT
				are_equal(1, t.suites_count());
			}


			test(RegisteringTestInOneTeeDoesNotAffectTheOther)
			{
				// INIT
				tee t1, t2;

				// ACT
				t1.add_test(&TestClass1::foo);

				// ASSERT
				are_equal(1, t1.suites_count());
				are_equal(0, t2.suites_count());
			}


			test(RegisteringSeveralTestForSameFixtureLeadsToOneTestSuite)
			{
				// INIT
				tee t;

				// ACT
				t.add_test(&TestClass1::foo);
				t.add_test(&TestClass1::bar);

				// ASSERT
				are_equal(1, t.suites_count());
			}


			test(TestSuitesCountEqualsFixturesInvolved)
			{
				// INIT
				tee t;

				// ACT
				t.add_test(&TestClass1::foo);
				t.add_test(&TestClass2::foo);

				// ASSERT
				are_equal(2, t.suites_count());

				// ACT
				t.add_test(&TestClass3::foo);

				// ASSERT
				are_equal(3, t.suites_count());
			}

						
		end_test_suite
	}
}
