#include <ut/tee.h>

namespace ut
{
	int tee::suites_count() const
	{
		return _suites.size();
	}
}