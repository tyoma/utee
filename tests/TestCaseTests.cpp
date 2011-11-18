#include <ut/test_case.h>

#include <ut/assert.h>
#include <ut/test.h>

namespace ut
{
	namespace
	{
		template <typename Fixture>
		typename test_case_impl<Fixture>::methods_ptr_t vmethods()
		{	return shared_ptr< test_case_impl<Fixture>::methods_list_t >(new test_case_impl<Fixture>::methods_list_t);	}

		template <typename Fixture>
		typename test_case_impl<Fixture>::methods_ptr_t vmethods(void (Fixture::*m1)())
		{	return typename test_case_impl<Fixture>::methods_ptr_t(new test_case_impl<Fixture>::methods_list_t(1, m1));	}

		template <typename Fixture>
		typename test_case_impl<Fixture>::methods_ptr_t vmethods(void (Fixture::*m1)(), void (Fixture::*m2)())
		{
			typename test_case_impl<Fixture>::methods_ptr_t m(vmethods<Fixture>());

			m->push_back(m1);
			m->push_back(m2);
			return m;
		}

		template <typename Fixture>
		typename test_case_impl<Fixture>::methods_ptr_t vmethods(void (Fixture::*m1)(), void (Fixture::*m2)(), void (Fixture::*m3)())
		{
			typename test_case_impl<Fixture>::methods_ptr_t m(vmethods<Fixture>());

			m->push_back(m1);
			m->push_back(m2);
			m->push_back(m3);
			return m;
		}

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
			int _to_be_initialized;

		public:
			UniqueValueTests()
				: _to_be_initialized(0)
			{	}

			void check_object_validity()
			{
				if (vvalue() != unique_value)
					throw 0;
			}

			virtual int vvalue()
			{	return unique_value;	}


			void set_to_square()
			{	_to_be_initialized = unique_value * unique_value;	}

			void check_if_squared()
			{
				if (_to_be_initialized != unique_value * unique_value)
					throw 0;
			}

			void init_1()
			{
				_to_be_initialized = unique_value;
			}

			void init_2()
			{
				++_to_be_initialized;
				_to_be_initialized *= 5;
			}

			void init_3()
			{
				++_to_be_initialized;
				_to_be_initialized *= 7;
			}

			void check_if_3_steps_initialized()
			{
				if (7 * (5 * (unique_value + 1) + 1) != _to_be_initialized)
					throw 0;
			}
		};


		begin_test_suite(TestCaseTests)
			test(TestCaseImplementationsUpcastableToTestCase)
			{
				// INIT / ACT
				test_case_impl<FooTests> ft(&FooTests::smoke, "x", vmethods<FooTests>(), vmethods<FooTests>());
				test_case_impl<BarTests> bt(&BarTests::smoke, "y", vmethods<BarTests>(), vmethods<BarTests>());

				// ACT / ASSERT (must compile)
				test_case &tc_ft = ft;
				test_case &tc_bt = bt;

				tc_ft, tc_bt;
			}


			test(TestCaseNameIsSetInCtor)
			{
				// INIT / ACT
				test_case_impl<FooTests> t1(&FooTests::smoke, "x", vmethods<FooTests>(), vmethods<FooTests>());
				test_case_impl<BarTests> t2(&BarTests::smoke, "Smoke", vmethods<BarTests>(), vmethods<BarTests>());
				test_case_impl<BarTests> t3(&BarTests::smoke, "z z z", vmethods<BarTests>(), vmethods<BarTests>());
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
				test_case_impl<FooTests> tf(&FooTests::always_fails, "", vmethods<FooTests>(), vmethods<FooTests>());
				test_case_impl<BarTests> tb(&BarTests::always_fails, "", vmethods<BarTests>(), vmethods<BarTests>());

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
				test_case_impl<FooTests> tf(&FooTests::always_passes, "", vmethods<FooTests>(), vmethods<FooTests>());
				test_case_impl<BarTests> tb(&BarTests::always_passes, "", vmethods<BarTests>(), vmethods<BarTests>());

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
				test_case_impl< UniqueValueTests<1> > t1(&UniqueValueTests<1>::check_object_validity, "",
					vmethods< UniqueValueTests<1> >(), vmethods< UniqueValueTests<1> >());
				test_case_impl< UniqueValueTests<101> > t2(&UniqueValueTests<101>::check_object_validity, "",
					vmethods< UniqueValueTests<101> >(), vmethods< UniqueValueTests<101> >());

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
				test_case_impl< UniqueValueTests<1> > t1(&UniqueValueTests<1>::check_object_validity, "",
					vmethods< UniqueValueTests<1> >(), vmethods< UniqueValueTests<1> >());
				test_case_impl<FooTests> t2(&FooTests::smoke, "", vmethods<FooTests>(), vmethods<FooTests>());
				test_case_impl<BarTests> t3(&BarTests::smoke, "", vmethods<BarTests>(), vmethods<BarTests>());
				test_case *tests[] = {	&t1, &t2, &t3,	};

				// ACT / ASSERT
				are_not_equal(std::string::npos, tests[0]->fixture_name().find("UniqueValueTests"));
				are_not_equal(std::string::npos, tests[1]->fixture_name().find("FooTests"));
				are_not_equal(std::string::npos, tests[2]->fixture_name().find("BarTests"));
			}


			test(InitializersAreCalledBeforeTestExecution)
			{
				// INIT
				test_case_impl< UniqueValueTests<3> > t1(&UniqueValueTests<3>::check_if_squared, "",
					vmethods(&UniqueValueTests<3>::set_to_square), vmethods< UniqueValueTests<3> >());
				test_case_impl< UniqueValueTests<12> > t2(&UniqueValueTests<12>::check_if_squared, "",
					vmethods(&UniqueValueTests<12>::set_to_square), vmethods< UniqueValueTests<12> >());

				// ACT
				test_result r1(t1.execute());
				test_result r2(t2.execute());

				// ASSERT
				is_true(r1.passed);
				is_true(r1.passed);
			}


			test(InitializersAreCalledInSequence)
			{
				// INIT
				test_case_impl< UniqueValueTests<3> > t1(&UniqueValueTests<3>::check_if_3_steps_initialized, "",
					vmethods(&UniqueValueTests<3>::init_3, &UniqueValueTests<3>::init_2, &UniqueValueTests<3>::init_1),
					vmethods< UniqueValueTests<3> >());
				test_case_impl< UniqueValueTests<11> > t2(&UniqueValueTests<11>::check_if_3_steps_initialized, "",
					vmethods(&UniqueValueTests<11>::init_3, &UniqueValueTests<11>::init_1, &UniqueValueTests<11>::init_2),
					vmethods< UniqueValueTests<11> >());
				test_case_impl< UniqueValueTests<13> > t3(&UniqueValueTests<13>::check_if_3_steps_initialized, "",
					vmethods(&UniqueValueTests<13>::init_1, &UniqueValueTests<13>::init_2, &UniqueValueTests<13>::init_3),
					vmethods< UniqueValueTests<13> >());

				// ACT
				test_result r1(t1.execute());
				test_result r2(t2.execute());
				test_result r3(t3.execute());

				// ASSERT
				is_false(r1.passed);
				is_false(r2.passed);
				is_true(r3.passed);
			}

		end_test_suite
	}
}
