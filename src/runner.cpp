#include "module.h"

#include <map>
#include <iostream>
#include <ut/registry.h>
#include <vector>

using namespace std;
using namespace ut;

namespace
{
	template <typename MapT>
	void accept_test_case(void *map, ut::test_case *i_test)
	{
		(*static_cast<MapT*>(map))[i_test->fixture_name().c_str()].push_back(i_test);
	}

	template <typename MapT>
	void load_test_cases(MapT &test_cases, module &m)
	{
		auto enumerator = m.address<EnumerateTestsF>("utee_enumerate_test_cases");

		enumerator(&test_cases, &accept_test_case<MapT>);
	}
}

int main(int argc, const char *argv[])
	try
{
	typedef vector<ut::test_case*> test_cases_t;
	typedef map<string, test_cases_t> test_cases_map_t;

	int passed = 0;
	int failed = 0;

	for (int i = 1; i < argc; ++i)
	{
		cout << "* Container: " << argv[i] << endl;

		module m(argv[i]);
		test_cases_map_t test_cases;

		load_test_cases(test_cases, m);

		for (test_cases_map_t::const_iterator i = test_cases.begin(); i != test_cases.end(); ++i)
		{
			cout << " * " << i->first << " ";
			for (test_cases_t::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
			{
				test_result result = (*j)->execute();
				if (result.initialized && result.passed && result.terminated)
				{
					++passed;
					cout << ">";
				}
				else
				{
					string title = !result.initialized ? "INIT FAILED" : !result.terminated ? "CLEANUP FAILED" : "FAILED";

					++failed;
					cout << endl
						<< "   [" << title << ": " << (*j)->name() << "]" << endl
						<< "   --- " << result.outcome << endl;
				}
			}
			cout << endl;
		}
		cout << endl;
	}

	cout << "PASSED: " << passed << endl;
	cout << "FAILED: " << failed << endl;
	return failed;
}
catch (exception &e)
{
	cerr << "Exception caught: " << e.what() << endl;
	return -1;
}
