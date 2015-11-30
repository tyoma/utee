#pragma once

#include <ut/exportable.h>

namespace ut
{
	namespace exportable
	{
		template <typename CharT>
		inline bool operator ==(const CharT *lhs, const exportable::immutable_basic_string<CharT> &rhs)
		{	return std::basic_string<CharT>(lhs) == rhs.c_str();	}
	}
}
