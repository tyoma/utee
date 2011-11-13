#include <ut/test_case.h>

#include <ut/assert.h>
#include <ut/test.h>

namespace ut
{
	namespace
	{
		class FooTests
		{
		public:
			void smoke()
			{
			}

			void always_fails()
			{
				throw 0;
			}

			void always_passes()
			{
			}
		};

		class Foo2Tests : public FooTests
		{
		public:
		};

		class BarTests
		{
		public:
			void smoke()
			{
			}

			void always_fails()
			{
				throw 0;
			}

			void always_passes()
			{
			}
		};

		template <int unique_value>
		class UniqueValueTests
		{
		public:
			void check_object_validity()
			{
				if (vvalue() != unique_value)
					throw 0;
			}

			virtual int vvalue()
			{	return unique_value;	}
		};


		begin_test_suite(TestCaseTests)
			test(TestCaseImplementationsUpcastableToTestCase)
			{
				// INIT / ACT
				test_case_impl<FooTests> ft(&FooTests::smoke, "x");
				test_case_impl<BarTests> bt(&BarTests::smoke, "y");

				// ACT / ASSERT (must compile)
				test_case &tc_ft = ft;
				test_case &tc_bt = bt;
			}


			test(TestCaseNameIsSetInCtor)
			{
				// INIT / ACT
				test_case_impl<FooTests> t1(&FooTests::smoke, "x");
				test_case_impl<BarTests> t2(&BarTests::smoke, "Smoke");
				test_case_impl<BarTests> t3(&BarTests::smoke, "z z z");
				test_case &tc_t1 = t1;
				test_case &tc_t2 = t2;
				test_case &tc_t3 = t3;

				// ACT / ASSERT
				are_equal("x", tc_t1.name());
				are_equal("Smoke", tc_t2.name());
				are_equal("z z z", tc_t3.name());
			}


			test(FailedExecutionSetsExecutionResult)
			{
				// INIT
				test_case_impl<FooTests> tf(&FooTests::always_fails, "");
				test_case_impl<BarTests> tb(&BarTests::always_fails, "");

				// ACT
				test_result rf(tf.execute());
				test_result rb(tb.execute());

				// ASSERT
				is_false(rf.passed);
				is_false(rb.passed);
			}


			test(PassedExecutionSetsExecutionResult)
			{
				// INIT
				test_case_impl<FooTests> tf(&FooTests::always_passes, "");
				test_case_impl<BarTests> tb(&BarTests::always_passes, "");

				// ACT
				test_result rf(tf.execute());
				test_result rb(tb.execute());

				// ASSERT
				is_true(rf.passed);
				is_true(rb.passed);
			}


			test(TestExecutionIsDoneForAValidFixtureObject)
			{
				// INIT
				test_case_impl< UniqueValueTests<1> > t1(&UniqueValueTests<1>::check_object_validity, "");
				test_case_impl< UniqueValueTests<101> > t2(&UniqueValueTests<101>::check_object_validity, "");

				// ACT
				test_result r1(t1.execute());
				test_result r2(t2.execute());

				// ASSERT
				is_true(r1.passed);
				is_true(r1.passed);
			}


			test(ReturnFixtureName)
			{
				// INIT
				test_case_impl< UniqueValueTests<1> > t1(&UniqueValueTests<1>::check_object_validity, "");
				test_case_impl<FooTests> t2(&FooTests::smoke, "");
				test_case_impl<BarTests> t3(&BarTests::smoke, "");
				test_case *tests[] = {	&t1, &t2, &t3,	};

				// ACT / ASSERT
				are_not_equal(std::string::npos, tests[0]->fixture_name().find("UniqueValueTests"));
				are_not_equal(std::string::npos, tests[1]->fixture_name().find("FooTests"));
				are_not_equal(std::string::npos, tests[2]->fixture_name().find("BarTests"));
			}


			test(InitializersAreCalledBeforeTestExecution)
			{
				// INIT

				// ACT
				// ASSERT
			}

		end_test_suite
	}
}
