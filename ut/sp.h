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

#ifndef __UTEE_UT_SP__
#define __UTEE_UT_SP__

namespace ut
{
	template <typename T>
	class shared_ptr
	{
		typedef unsigned int ref_counter_t;

		T *_p;
		ref_counter_t *_refs;

		void assign(T *p, ref_counter_t *refs) throw();
		void release() throw();

	public:
		explicit shared_ptr(T *p = 0);
		shared_ptr(const shared_ptr &other) throw();
		~shared_ptr() throw();

		T *operator ->() const;
		T &operator *() const;
		const shared_ptr &operator =(const shared_ptr &rhs) throw();
	};


	template <typename T>
	inline shared_ptr<T>::shared_ptr(T *p)
	{	assign(p, p ? new ref_counter_t() : 0);	}

	template <typename T>
	inline shared_ptr<T>::shared_ptr(const shared_ptr &other) throw()
	{	assign(other._p, other._refs);	}

	template <typename T>
	inline shared_ptr<T>::~shared_ptr() throw()
	{	release();	}

	template <typename T>
	inline T *shared_ptr<T>::operator ->() const
	{	return _p;	}

	template <typename T>
	inline T &shared_ptr<T>::operator *() const
	{	return *_p;	}

	template <typename T>
	inline const shared_ptr<T> &shared_ptr<T>::operator =(const shared_ptr &rhs) throw()
	{
		if (this != &rhs)
			release(), assign(rhs._p, rhs._refs);
		return *this;
	}

	template <typename T>
	inline void shared_ptr<T>::assign(T *p, ref_counter_t *refs) throw()
	{
		_p = p, _refs = refs;
		if (_p)
			++*_refs;
	}

	template <typename T>
	inline void shared_ptr<T>::release() throw()
	{
		if (_p && !--*_refs)
			delete _p, delete _refs;
	}
}

#endif	// __UTEE_UT_SP__
