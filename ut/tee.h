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
#include "test_case.h"

#include <functional>
#include <set>
#include <string>
#include <vector>

namespace ut
{
	struct type_info_less : std::binary_function<const type_info *, const type_info *, bool>
	{
		bool operator ()(const type_info *lhs, const type_info *rhs) const;
	};

	class tee
	{
		std::vector< shared_ptr<test_case> > _test_cases;
		std::set<const type_info *, type_info_less> _suites;

	public:
		typedef std::vector< shared_ptr<test_case> >::const_iterator const_iterator;

	public:
		template <typename FixtureT>
		void add_test(void (FixtureT::*method)(), const char *name);

		int suites_count() const;
		int tests_count() const;
		const_iterator tests_begin() const;
		const_iterator tests_end() const;
	};


	inline bool type_info_less::operator ()(const type_info *lhs, const type_info *rhs) const
	{	return !!lhs->before(*rhs);	}


	template <typename FixtureT>
	inline void tee::add_test(void (FixtureT::*method)(), const char *name)
	{
		typedef test_case_impl<FixtureT> test_case;
		typedef typename test_case_impl<FixtureT>::methods_list_t methods_list_t;
		typedef typename test_case_impl<FixtureT>::methods_ptr_t methods_ptr_t;

		_suites.insert(&typeid(FixtureT));
		_test_cases.push_back(shared_ptr<test_case>(new test_case_impl<FixtureT>(method, name,
			methods_ptr_t(new methods_list_t()), methods_ptr_t(new methods_list_t()))));
	}
}

#endif	// __UTEE_UT_TEE__
