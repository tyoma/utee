#pragma once

#include <ut/assert.h>
#include <ut/exportable.h>

namespace ut
{
	template <typename T, size_t n>
	inline void are_equal(T (&i_lhs)[n], const exportable::immutable_string &i_rhs, const LocationInfo &i_location)
	{	are_equal(i_lhs, std::string(i_rhs.c_str()), i_location);	}
}
