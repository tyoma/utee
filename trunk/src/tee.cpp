#include <ut/tee.h>

namespace ut
{
	tee &tee_instance()
	{
		static tee g_instance;

		return g_instance;
	}

	int tee::suites_count() const
	{	return _suites.size();	}

	int tee::tests_count() const
	{	return _test_cases.size();	}

	tee::const_iterator tee::tests_begin() const
	{	return _test_cases.begin();	}

	tee::const_iterator tee::tests_end() const
	{	return _test_cases.end();	}
}
