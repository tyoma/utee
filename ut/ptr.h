#pragma once

namespace ut
{
	template <typename T, typename CounterT>
	class shared_ptr;

	template <typename U, typename T, typename CounterT>
	shared_ptr<U, CounterT> static_pointer_cast(const shared_ptr<T, CounterT> &from);

	template <typename T, typename CounterT = unsigned int>
	class shared_ptr
	{
	public:
		explicit shared_ptr(T *p);
		shared_ptr(const shared_ptr &other) throw();
		template <typename U>
		shared_ptr(const shared_ptr<U, CounterT> &other) throw();
		~shared_ptr() throw();

		T *get() const throw();
		void reset() throw();

		T *operator ->() const throw();
		T &operator *() const throw();

		const shared_ptr &operator =(const shared_ptr &other) throw();

	private:
		shared_ptr(T *p, CounterT *refcount) throw();

	private:
		T *_ptr;
		CounterT *_refcount;

		template <typename U, typename Counter2T>
		friend class shared_ptr;

		template <typename U, typename V, typename Counter2T>
		friend shared_ptr<U, Counter2T> static_pointer_cast(const shared_ptr<V, Counter2T> &from);
	};

	template <typename T, typename CounterT>
	inline shared_ptr<T, CounterT>::shared_ptr(T *p)
		: _ptr(p), _refcount(new CounterT(1))
	{	}

	template <typename T, typename CounterT>
	inline shared_ptr<T, CounterT>::shared_ptr(const shared_ptr &other) throw()
		: _ptr(other._ptr), _refcount(other._refcount)
	{
		if (_refcount)
			++*_refcount;
	}

	template <typename T, typename CounterT>
	template <typename U>
	inline shared_ptr<T, CounterT>::shared_ptr(const shared_ptr<U, CounterT> &other) throw()
		: _ptr(other._ptr), _refcount(other._refcount)
	{
		if (_refcount)
			++*_refcount;
	}

	template <typename T, typename CounterT>
	inline shared_ptr<T, CounterT>::shared_ptr(T *p, CounterT *refcount) throw()
		: _ptr(p), _refcount(refcount)
	{
		if (_refcount)
			++*_refcount;
	}

	template <typename T, typename CounterT>
	inline shared_ptr<T, CounterT>::~shared_ptr() throw()
	{	reset();	}

	template <typename T, typename CounterT>
	inline void shared_ptr<T, CounterT>::reset() throw()
	{
		if (_refcount && !--*_refcount)
			delete _ptr, delete _refcount;
		_ptr = 0, _refcount = 0;
	}

	template <typename T, typename CounterT>
	inline T *shared_ptr<T, CounterT>::get() const throw()
	{	return _ptr;	}

	template <typename T, typename CounterT>
	inline T *shared_ptr<T, CounterT>::operator ->() const throw()
	{	return _ptr;	}

	template <typename T, typename CounterT>
	inline const shared_ptr<T, CounterT> &shared_ptr<T, CounterT>::operator =(const shared_ptr &other) throw()
	{
		if (&other != this)
		{
			reset();
			_ptr = other._ptr;
			_refcount = other._refcount;
			if (_refcount)
				++*_refcount;
		}
		return *this;
	}



	template <typename U, typename T, typename CounterT>
	inline shared_ptr<U, CounterT> static_pointer_cast(const shared_ptr<T, CounterT> &from)
	{	return shared_ptr<U, CounterT>(static_cast<U *>(from._ptr), from._refcount);	}
}
