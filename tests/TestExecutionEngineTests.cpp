#include <ut/tee.h>

#include <ut/assert.h>
#include <ut/test.h>

namespace ut
{
	namespace tests
	{
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

				// ASSERT
				are_equal(1, t.suites_count());
			}


			test(RegisteringTestInOneTeeDoesNotAffectTheOther)
			{
				// INIT
				tee t1, t2;

				// ACT

				// ASSERT
				are_equal(1, t1.suites_count());
				are_equal(0, t2.suites_count());
			}

						
		end_test_suite
	}
}
