#pragma once

#include <memory>
#include <stdexcept>

namespace ut
{
	class module
	{
	public:
		module(const char *path);

		template <typename F>
		F address(const char *name);

	private:
		void *address(const char *name);

	private:
		std::shared_ptr<void> _module;
	};


	template <typename F>
	inline F module::address(const char *name)
	{  return reinterpret_cast<F>(address(name)); }
}
