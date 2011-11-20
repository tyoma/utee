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

#ifndef __UTEE_UT_TEST__
#define __UTEE_UT_TEST__

#ifdef USE_MSVCQT

#define begin_test_suite(__test_suite)\
[Microsoft::VisualStudio::TestTools::UnitTesting::TestClass]\
public ref class __test_suite\
{\
public:

#define test(__test)\
	[Microsoft::VisualStudio::TestTools::UnitTesting::TestMethod]\
	void __test()

#define end_test_suite	};

#else	// USE_MSVCQT

#include "tee.h"

namespace ut
{
	template <class FixtureT, void (FixtureT::*metadata_provider(const char *&))()>
	struct test_case_registrar
	{
		test_case_registrar();
	};


	tee &tee_instance();

	template <class FixtureT, void (FixtureT::*metadata_provider(const char *&))()>
	inline test_case_registrar<FixtureT, metadata_provider>::test_case_registrar()
	{
		typedef void (FixtureT::*test_case_method_t)();

		const char *name = 0;
		test_case_method_t method = metadata_provider(name);

		tee_instance().add_test<FixtureT>(method, name);
	}
}

#define begin_test_suite(__test_suite)\
namespace	{\
	class __test_suite\
	{\
		typedef __test_suite this_suite_class;\
	public:

#define test(__test)\
		static void (this_suite_class::*__##__test##_meta(const char *&name))()\
		{	return name = #__test, &this_suite_class::__test;	}\
		ut::test_case_registrar<this_suite_class, &this_suite_class::__##__test##_meta>	__##__test##_registrar;\
		void __test()

#define end_test_suite\
	} g_suite;\
}

#endif	// USE_MSVCQT

#endif	// __UTEE_UT_TEST__
