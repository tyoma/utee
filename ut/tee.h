//	Copyright (C) 2011 by Artem A. Gevorkyan (gevorkyan.org)
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#ifndef __UTEE_UT_TEE__
#define __UTEE_UT_TEE__

#include "sp.h"

#include <functional>
#include <set>
#include <string>

namespace ut
{
	struct type_info_less : std::binary_function<const type_info *, const type_info *, bool>
	{
		bool operator ()(const type_info *lhs, const type_info *rhs) const;
	};

	struct test_result
	{
		bool passed;
		std::string outcome;
	};

	struct test_case
	{
		virtual ~test_case()	{	}
		virtual std::string fixture_name() const = 0;
		virtual std::string name() const = 0;
		virtual test_result execute() = 0;
	};

	struct fixture
	{
		virtual ~fixture() {	}
		virtual std::string name() const = 0;
		virtual int test_cases_count() const = 0;
		virtual shared_ptr<test_case> get_test(int i) const = 0;
	};

	class tee
	{
		std::set<const type_info *, type_info_less> _suites;

	public:
		int suites_count() const;
		shared_ptr<fixture> get_fixture(int i) const;

		template <typename FixtureT>
		void add_test(void (FixtureT::*method)());
	};


	inline bool type_info_less::operator ()(const type_info *lhs, const type_info *rhs) const
	{	return !!lhs->before(*rhs);	}


	template <typename FixtureT>
	inline void tee::add_test(void (FixtureT::*method)())
	{
		_suites.insert(&typeid(FixtureT));
	}
}

#endif	// __UTEE_UT_TEE__