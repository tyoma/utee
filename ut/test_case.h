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

#include <string>
#include <vector>

namespace ut
{
	struct test_result
	{
		bool initialized : 1;
		bool passed : 1;
		bool terminated : 1;

		std::string outcome;
	};

	struct test_case
	{
		virtual ~test_case()	{	}
		virtual std::string fixture_name() const = 0;
		virtual std::string name() const = 0;
		virtual test_result execute() = 0;
	};

	template <typename Fixture>
	class test_case_impl : public test_case
	{
		typedef void (Fixture::*_method_t)();
		typedef std::vector<_method_t> _methods_list_t;
		typedef shared_ptr<_methods_list_t> _methods_ptr_t;
		
		const _method_t _method;
		const std::string _name;
		_methods_ptr_t _initializers;

		const test_case_impl &operator =(const test_case_impl &rhs);

	public:
		typedef _method_t method_t;
		typedef _methods_list_t methods_list_t;
		typedef _methods_ptr_t methods_ptr_t;

	public:
		test_case_impl(method_t method, const std::string &name, methods_ptr_t initializers, methods_ptr_t terminators);

		virtual std::string fixture_name() const;
		virtual std::string name() const;
		virtual test_result execute();
	};


	template <typename Fixture>
	inline test_case_impl<Fixture>::test_case_impl(method_t method, const std::string &name,
		methods_ptr_t initializers, methods_ptr_t terminators)
		: _method(method), _name(name), _initializers(initializers)
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
		test_result r = { false, false, false };

		try
		{
			Fixture f;

			for (_methods_list_t::const_iterator i = _initializers->begin(); i != _initializers->end(); ++i)
				(f.**i)();

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
