#include <ut/test_case.h>

#include "helpers.h"

#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace ut
{
	namespace tests
	{
		namespace
		{
			template <typename FixtureT>
			shared_ptr< setup_impl<FixtureT> > setup_fixture()
			{	return shared_ptr< setup_impl<FixtureT> >(new setup_impl<FixtureT>());	}

			class FooTests
			{
			public:
				static const char *__suite_name() {	return "FooTests";	}

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
				static const char *__suite_name() {	return "Foo2Tests";	}
			};

			class BarTests
			{
			public:
				static const char *__suite_name() {	return "BarTests";	}

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
				static const char *__suite_name() {	return "UniqueValueTests";	}

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
		}


		begin_test_suite(TestCaseTests)
			test( TestCaseImplementationsUpcastableToTestCase )
			{
				// INIT / ACT
				test_case_impl<FooTests> ft(&FooTests::smoke, "x", setup_fixture<FooTests>());
				test_case_impl<BarTests> bt(&BarTests::smoke, "y", setup_fixture<BarTests>());

				// ACT / ASSERT (must compile)
				test_case &tc_ft = ft;
				test_case &tc_bt = bt;
				
				// ASSERT
				assert_not_equal(&tc_ft, &tc_bt);
			}


			test( TestCaseNameIsSetInCtor )
			{
				// INIT / ACT
				test_case_impl<FooTests> t1(&FooTests::smoke, "x", setup_fixture<FooTests>());
				test_case_impl<BarTests> t2(&BarTests::smoke, "Smoke", setup_fixture<BarTests>());
				test_case_impl<BarTests> t3(&BarTests::smoke, "z z z", setup_fixture<BarTests>());
				test_case &tc_t1 = t1;
				test_case &tc_t2 = t2;
				test_case &tc_t3 = t3;

				// ACT / ASSERT
				assert_equal("x", tc_t1.name());
				assert_equal("Smoke", tc_t2.name());
				assert_equal("z z z", tc_t3.name());
			}


			test( FailedExecutionSetsExecutionResult )
			{
				// INIT
				test_case_impl<FooTests> tf(&FooTests::always_fails, "", setup_fixture<FooTests>());
				test_case_impl<BarTests> tb(&BarTests::always_fails, "", setup_fixture<BarTests>());

				// ACT
				test_result rf(tf.execute());
				test_result rb(tb.execute());

				// ASSERT
				assert_is_false(rf.passed);
				assert_is_false(rb.passed);
			}


			test( PassedExecutionSetsExecutionResult )
			{
				// INIT
				test_case_impl<FooTests> tf(&FooTests::always_passes, "", setup_fixture<FooTests>());
				test_case_impl<BarTests> tb(&BarTests::always_passes, "", setup_fixture<BarTests>());

				// ACT
				test_result rf(tf.execute());
				test_result rb(tb.execute());

				// ASSERT
				assert_is_true(rf.passed);
				assert_is_true(rb.passed);
			}


			test( TestExecutionIsDoneForAValidFixtureObject )
			{
				// INIT
				test_case_impl< UniqueValueTests<1> > t1(&UniqueValueTests<1>::check_object_validity, "",
					setup_fixture< UniqueValueTests<1> >());
				test_case_impl< UniqueValueTests<101> > t2(&UniqueValueTests<101>::check_object_validity, "",
					setup_fixture< UniqueValueTests<101> >());

				// ACT
				test_result r1(t1.execute());
				test_result r2(t2.execute());

				// ASSERT
				assert_is_true(r1.passed);
				assert_is_true(r1.passed);
			}


			test( ReturnFixtureName )
			{
				// INIT
				test_case_impl< UniqueValueTests<1> > t1(&UniqueValueTests<1>::check_object_validity, "",
					setup_fixture< UniqueValueTests<1> >());
				test_case_impl<FooTests> t2(&FooTests::smoke, "", setup_fixture<FooTests>());
				test_case_impl<BarTests> t3(&BarTests::smoke, "", setup_fixture<BarTests>());
				test_case *tests[] = {	&t1, &t2, &t3,	};

				// ACT / ASSERT
				assert_equal("UniqueValueTests", tests[0]->fixture_name());
				assert_equal("FooTests", tests[1]->fixture_name());
				assert_equal("BarTests", tests[2]->fixture_name());
			}


			test( InitializersAreCalledBeforeTestExecution )
			{
				// INIT
				shared_ptr< setup_impl< UniqueValueTests<3> > > f1 = setup_fixture< UniqueValueTests<3> >();
				shared_ptr< setup_impl< UniqueValueTests<12> > > f2 = setup_fixture< UniqueValueTests<12> >();

				// INIT / ACT
				f1->add_init_method(&UniqueValueTests<3>::set_to_square);
				f2->add_init_method(&UniqueValueTests<12>::set_to_square);

				test_case_impl< UniqueValueTests<3> > t1(&UniqueValueTests<3>::check_if_squared, "", f1);
				test_case_impl< UniqueValueTests<12> > t2(&UniqueValueTests<12>::check_if_squared, "", f2);

				// ACT
				test_result r1(t1.execute());
				test_result r2(t2.execute());

				// ASSERT
				assert_is_true(r1.passed);
				assert_is_true(r1.passed);
			}


			test( InitializersAreCalledInSequence )
			{
				// INIT
				shared_ptr< setup_impl< UniqueValueTests<3> > > f1 = setup_fixture< UniqueValueTests<3> >();
				shared_ptr< setup_impl< UniqueValueTests<11> > > f2 = setup_fixture< UniqueValueTests<11> >();
				shared_ptr< setup_impl< UniqueValueTests<13> > > f3 = setup_fixture< UniqueValueTests<13> >();

				// INIT / ACT
				f1->add_init_method(&UniqueValueTests<3>::init_3);
				f1->add_init_method(&UniqueValueTests<3>::init_2);
				f1->add_init_method(&UniqueValueTests<3>::init_1);

				f2->add_init_method(&UniqueValueTests<11>::init_3);
				f2->add_init_method(&UniqueValueTests<11>::init_1);
				f2->add_init_method(&UniqueValueTests<11>::init_2);

				f3->add_init_method(&UniqueValueTests<13>::init_1);
				f3->add_init_method(&UniqueValueTests<13>::init_2);
				f3->add_init_method(&UniqueValueTests<13>::init_3);

				test_case_impl< UniqueValueTests<3> > t1(&UniqueValueTests<3>::check_if_3_steps_initialized, "", f1);
				test_case_impl< UniqueValueTests<11> > t2(&UniqueValueTests<11>::check_if_3_steps_initialized, "", f2);
				test_case_impl< UniqueValueTests<13> > t3(&UniqueValueTests<13>::check_if_3_steps_initialized, "", f3);

				// ACT
				test_result r1(t1.execute());
				test_result r2(t2.execute());
				test_result r3(t3.execute());

				// ASSERT
				assert_is_false(r1.passed);
				assert_is_false(r2.passed);
				assert_is_true(r3.passed);
			}

		end_test_suite
	}
}
