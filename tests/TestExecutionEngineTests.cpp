#include <ut/tee.h>

#include <ut/assert.h>
#include <ut/test.h>
#include <iterator>

using namespace std;

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
				t.add_test(&TestClass1::foo, "");

				// ASSERT
				are_equal(1, t.suites_count());
			}


			test(RegisteringTestInOneTeeDoesNotAffectTheOther)
			{
				// INIT
				tee t1, t2;

				// ACT
				t1.add_test(&TestClass1::foo, "");

				// ASSERT
				are_equal(1, t1.suites_count());
				are_equal(0, t2.suites_count());
			}


			test(RegisteringSeveralTestForSameFixtureLeadsToOneTestSuite)
			{
				// INIT
				tee t;

				// ACT
				t.add_test(&TestClass1::foo, "");
				t.add_test(&TestClass1::bar, "");

				// ASSERT
				are_equal(1, t.suites_count());
			}


			test(TestSuitesCountEqualsFixturesInvolved)
			{
				// INIT
				tee t;

				// ACT
				t.add_test(&TestClass1::foo, "");
				t.add_test(&TestClass2::foo, "");

				// ASSERT
				are_equal(2, t.suites_count());

				// ACT
				t.add_test(&TestClass3::foo, "");

				// ASSERT
				are_equal(3, t.suites_count());
			}


			test(TestCasesListIsEmptyAtConstruction)
			{
				// INIT
				tee t;

				// ACT / ASSERT
				are_equal(0, t.tests_count());
				are_equal(0, distance(t.tests_begin(), t.tests_begin()));
			}


			test(AddingTestCasesMakesTestsListNonEmpty)
			{
				// INIT
				tee t1, t2;

				// ACT
				t1.add_test(&TestClass1::foo, "");
				t2.add_test(&TestClass1::bar, "");
				t2.add_test(&TestClass2::foo, "");

				// ACT / ASSERT
				are_equal(1, t1.tests_count());
				are_equal(1, distance(t1.tests_begin(), t1.tests_end()));
				are_equal(2, t2.tests_count());
				are_equal(2, distance(t2.tests_begin(), t2.tests_end()));
			}


			test(AddingTestCasesRevealsThemInTee)
			{
				// INIT
				tee t1, t2, t3;

				// ACT
				t1.add_test(&TestClass1::foo, "foo");
				t2.add_test(&TestClass1::bar, "bar");
				t3.add_test(&TestClass2::foo, "FOO foo");

				shared_ptr<test_case> TestClass1_foo = *t1.tests_begin();
				shared_ptr<test_case> TestClass1_bar = *t2.tests_begin();
				shared_ptr<test_case> TestClass2_foo = *t3.tests_begin();

				// ACT / ASSERT
				are_not_equal(string::npos, TestClass1_foo->fixture_name().find("TestClass1"));
				are_equal("foo", TestClass1_foo->name());
				are_not_equal(string::npos, TestClass1_bar->fixture_name().find("TestClass1"));
				are_equal("bar", TestClass1_bar->name());
				are_not_equal(string::npos, TestClass2_foo->fixture_name().find("TestClass2"));
				are_equal("FOO foo", TestClass2_foo->name());
			}


			test(TestCaseAdditionOrderIsPreserved)
			{
				// INIT
				tee t1, t2;

				// ACT
				t1.add_test(&TestClass1::foo, "foo");
				t1.add_test(&TestClass1::bar, "bar");
				t1.add_test(&TestClass2::foo, "FOO foo");
				t2.add_test(&TestClass2::foo, "FOO foo");
				t2.add_test(&TestClass1::foo, "foo");

				// ACT / ASSERT
				are_not_equal(string::npos, (*(t1.tests_begin() + 0))->fixture_name().find("TestClass1"));
				are_equal("foo", (*(t1.tests_begin() + 0))->name());
				are_not_equal(string::npos, (*(t1.tests_begin() + 1))->fixture_name().find("TestClass1"));
				are_equal("bar", (*(t1.tests_begin() + 1))->name());
				are_not_equal(string::npos, (*(t1.tests_begin() + 2))->fixture_name().find("TestClass2"));
				are_equal("FOO foo", (*(t1.tests_begin() + 2))->name());

				are_not_equal(string::npos, (*(t2.tests_begin() + 0))->fixture_name().find("TestClass2"));
				are_equal("FOO foo", (*(t2.tests_begin() + 0))->name());
				are_not_equal(string::npos, (*(t2.tests_begin() + 1))->fixture_name().find("TestClass1"));
				are_equal("foo", (*(t2.tests_begin() + 1))->name());
			}


			test(TestsAreOnlyAddedOnceBasedOnTypeAndName)
			{
				// INIT
				tee t1, t2;

				// ACT
				t1.add_test(&TestClass1::foo, "foo");
				t1.add_test(&TestClass1::bar, "bar");
				t1.add_test(&TestClass1::foo, "foo");
				t2.add_test(&TestClass2::foo, "FOO foo");
				t2.add_test(&TestClass2::foo, "FOO foo");


				// ACT / ASSERT
				are_equal(2, t1.tests_count());
				are_equal(2, distance(t1.tests_begin(), t1.tests_end()));
				are_not_equal(string::npos, (*(t1.tests_begin() + 0))->fixture_name().find("TestClass1"));
				are_equal("foo", (*(t1.tests_begin() + 0))->name());
				are_not_equal(string::npos, (*(t1.tests_begin() + 1))->fixture_name().find("TestClass1"));
				are_equal("bar", (*(t1.tests_begin() + 1))->name());

				are_equal(1, t2.tests_count());
				are_equal(1, distance(t2.tests_begin(), t2.tests_end()));
				are_not_equal(string::npos, (*(t2.tests_begin() + 0))->fixture_name().find("TestClass2"));
				are_equal("FOO foo", (*(t2.tests_begin() + 0))->name());
			}
		end_test_suite
	}
}
