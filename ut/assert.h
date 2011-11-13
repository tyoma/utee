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

#ifndef __UTEE_UT_ASSERT__
#define __UTEE_UT_ASSERT__

namespace ut
{
#ifdef USE_MSVCQT

template <typename T1, typename T2>
inline void are_equal(const T1 &lhs, const T2 &rhs)
{	Microsoft::VisualStudio::TestTools::UnitTesting::Assert::IsTrue(lhs == rhs);	}

template <typename T1, typename T2>
inline void are_not_equal(const T1 &lhs, const T2 &rhs)
{	Microsoft::VisualStudio::TestTools::UnitTesting::Assert::IsFalse(lhs == rhs);	}

inline void is_true(bool value)
{	Microsoft::VisualStudio::TestTools::UnitTesting::Assert::IsTrue(value);	}

inline void is_false(bool value)
{	Microsoft::VisualStudio::TestTools::UnitTesting::Assert::IsFalse(value);	}

#else	// USE_MSVCQT

#endif	// USE_MSVCQT
}

#endif	// __UTEE_UT_ASSERT__
