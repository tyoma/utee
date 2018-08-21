#include "module.h"

#include <string>
#ifdef _WIN32
#include <windows.h>
#elif __unix__
#include <dlfcn.h>
#else
#include <dlfcn.h>
#endif

using namespace std;

namespace ut
{
	module::module(const char *path)
#ifdef _WIN32
		: _module(::LoadLibraryA(path))
#elif __unix__
		: _module(::dlopen(path, RTLD_NOW))
#else
		: _module(::dlopen(path, RTLD_NOW))
#endif
	{
		if (!_module)
			throw std::runtime_error(string("Cannot load module '") + path + "'!");
	}

	module::~module()
#ifdef _WIN32
	{	::FreeLibrary(static_cast<HMODULE>(_module));	}
#elif __unix__
	{	::dlclose(_module);	}
#else
	{	::dlclose(_module);	}
#endif

	void *module::address(const char *name)
#ifdef _WIN32
	{	return ::GetProcAddress(static_cast<HMODULE>(_module), name);	}
#elif __unix__
	{	return ::dlsym(_module, name);	}
#else
	{	return ::dlsym(_module, name);	}
#endif
}
