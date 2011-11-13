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

#ifndef __UTEE_UT_TEST_CASE__
#define __UTEE_UT_TEST_CASE__

#include "sp.h"
#include "tee.h"

#include <string>
#include <vector>

namespace ut
{
	template <typename Fixture>
	class test_case_impl : public test_case
	{
		typedef void (Fixture::*test_method_t)();
		
		const test_method_t _method;
		const std::string _name;

	public:
		typedef std::vector<void (Fixture::*)()> methods_list_t;

	public:
		test_case_impl(test_method_t method, const std::string &name,
			shared_ptr<methods_list_t> initializers = shared_ptr<methods_list_t>(),
			shared_ptr<methods_list_t> terminators = shared_ptr<methods_list_t>());

		virtual std::string fixture_name() const;
		virtual std::string name() const;
		virtual test_result execute();
	};


	template <typename Fixture>
	inline test_case_impl<Fixture>::test_case_impl(test_method_t method, const std::string &name,
		shared_ptr<methods_list_t> initializers, shared_ptr<methods_list_t> terminators)
		: _method(method), _name(name)
	{	}

	template <typename Fixture>
	inline std::string test_case_impl<Fixture>::fixture_name() const
	{	return typeid(Fixture).name();	}

	template <typename Fixture>
	inline std::string test_case_impl<Fixture>::name() const
	{	return _name;	}

	template <typename Fixture>
	inline test_result test_case_impl<Fixture>::execute()
	{
		test_result r = { false };

		try
		{
			Fixture f;

			(f.*_method)();
			r.passed = true;
		}
		catch (...)
		{
		}
		return r;
	}
}

#endif	// __UTEE_UT_TEST_CASE__
