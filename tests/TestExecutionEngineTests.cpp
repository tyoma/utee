#include <ut/registry.h>

#include "helpers.h"

#include <iterator>
#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace ut
{
	namespace tests
	{
		namespace
		{
			class TestClass1
			{
			public:
				static const char *__suite_name() {	return "TestClass1";	}
				static const char *__suite_id() {	return "abc";	}

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
				static const char *__suite_name() {	return "TestClass2";	}
				static const char *__suite_id() {	return "abc1";	}

				void foo()
				{
				}
			};


			class TestClass3
			{
			public:
				static const char *__suite_name() {	return "TestClass3";	}
				static const char *__suite_id() {	return "xyz";	}

				void foo()
				{
				}
			};
		}

		begin_test_suite(TestExecutionEngineTests)
			test( TestsRegistryIsEmptyByDefault )
			{
				// INIT
				registry t;
				const registry &const_t(t);

				// ACT / ASSERT
				assert_equal(0, const_t.tests_count());
			}


			test( RegisteringTestChangesTestsCount )
			{
				// INIT
				registry t;

				// ACT
				t.add_test(&TestClass1::foo, "");

				// ASSERT
				assert_equal(1, t.tests_count());
			}


			test( RegisteringTestsInOneRegistryDoesNotAffectAnother )
			{
				// INIT
				registry t1, t2;

				// ACT
				t1.add_test(&TestClass1::foo, "");

				// ASSERT
				assert_equal(1, t1.tests_count());
				assert_equal(0, t2.tests_count());
			}


			test( TestsFromDifferentFixturesAreCountedAsTotal )
			{
				// INIT
				registry t;

				// ACT
				t.add_test(&TestClass1::foo, "x");
				t.add_test(&TestClass1::bar, "y");

				// ASSERT
				assert_equal(2, t.tests_count());

				// ACT
				t.add_test(&TestClass3::foo, "z");

				// ASSERT
				assert_equal(3, t.tests_count());
			}


			test(AddingTestCasesMakesTestsListNonEmpty)
			{
				// INIT
				registry t1, t2;

				// ACT
				t1.add_test(&TestClass1::foo, "x");
				t2.add_test(&TestClass1::bar, "x");
				t2.add_test(&TestClass2::foo, "y");

				// ACT / ASSERT
				assert_equal(1, t1.tests_count());
				assert_equal(1, distance(t1.tests_begin(), t1.tests_end()));
				assert_equal(2, t2.tests_count());
				assert_equal(2, distance(t2.tests_begin(), t2.tests_end()));
			}


			test(AddingTestCasesRevealsThemInTee)
			{
				// INIT
				registry t1, t2, t3;

				// ACT
				t1.add_test(&TestClass1::foo, "foo");
				t2.add_test(&TestClass1::bar, "bar");
				t3.add_test(&TestClass2::foo, "FOO foo");

				shared_ptr<test_case> TestClass1_foo = *t1.tests_begin();
				shared_ptr<test_case> TestClass1_bar = *t2.tests_begin();
				shared_ptr<test_case> TestClass2_foo = *t3.tests_begin();

				// ACT / ASSERT
				assert_equal("TestClass1", TestClass1_foo->fixture_name());
				assert_equal("foo", TestClass1_foo->name());
				assert_equal("TestClass1", TestClass1_bar->fixture_name());
				assert_equal("bar", TestClass1_bar->name());
				assert_equal("TestClass2", TestClass2_foo->fixture_name());
				assert_equal("FOO foo", TestClass2_foo->name());
			}


			test( TestCaseAdditionOrderIsPreserved )
			{
				// INIT
				registry t1, t2;

				// ACT
				t1.add_test(&TestClass1::foo, "foo");
				t1.add_test(&TestClass1::bar, "bar");
				t1.add_test(&TestClass2::foo, "FOO foo");
				t2.add_test(&TestClass2::foo, "FOO baz");
				t2.add_test(&TestClass1::foo, "foo");

				// ACT / ASSERT
				registry::const_iterator i = t1.tests_begin();

				assert_equal("TestClass1", (*i)->fixture_name());
				assert_equal("foo", (*i)->name());
				++i;
				assert_equal("TestClass1", (*i)->fixture_name());
				assert_equal("bar", (*i)->name());
				++i;
				assert_equal("TestClass2", (*i)->fixture_name());
				assert_equal("FOO foo", (*i)->name());

				i = t2.tests_begin();

				assert_equal("TestClass2", (*i)->fixture_name());
				assert_equal("FOO baz", (*i)->name());
				++i;
				assert_equal("TestClass1", (*i)->fixture_name());
				assert_equal("foo", (*i)->name());
			}


			test(TestsAreOnlyAddedOnceBasedOnTypeAndName)
			{
				// INIT
				registry t1, t2;

				// ACT
				t1.add_test(&TestClass1::foo, "foo");
				t1.add_test(&TestClass1::bar, "bar");
				t1.add_test(&TestClass1::foo, "foo");
				t2.add_test(&TestClass2::foo, "FOO foo");
				t2.add_test(&TestClass2::foo, "FOO foo");


				// ACT / ASSERT
				assert_equal(2, t1.tests_count());
				assert_equal(2, distance(t1.tests_begin(), t1.tests_end()));

				registry::const_iterator i = t1.tests_begin();

				assert_equal("TestClass1", (*i)->fixture_name());
				assert_equal("foo", (*i)->name());
				++i;
				assert_equal("TestClass1", (*i)->fixture_name());
				assert_equal("bar", (*i)->name());

				assert_equal(1, t2.tests_count());
				assert_equal(1, distance(t2.tests_begin(), t2.tests_end()));

				i = t2.tests_begin();

				assert_equal("TestClass2", (*i)->fixture_name());
				assert_equal("FOO foo", (*i)->name());
			}
		end_test_suite
	}
}
