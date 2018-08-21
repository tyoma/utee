#pragma once

#include <stdexcept>

namespace ut
{
	class module
	{
	public:
		explicit module(const char *path);
		~module();

		template <typename F>
		F address(const char *name);

	private:
		module(const module &other);
		const module &operator =(const module &rhs);
		void *address(const char *name);

	private:
		void *_module;
	};


	template <typename F>
	inline F module::address(const char *name)
	{  return reinterpret_cast<F>(address(name)); }
}
