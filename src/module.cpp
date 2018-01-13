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
		: _module(::LoadLibraryA(path), &::FreeLibrary)
#elif __unix__
		: _module(::dlopen(path, RTLD_NOW), &::dlclose)
#else
		: _module(::dlopen(path, RTLD_NOW), &::dlclose)
#endif
	{
		if (!_module)
			throw std::runtime_error(string("Cannot load module '") + path + "'!");
	}

	void *module::address(const char *name)
#ifdef _WIN32
	{  return ::GetProcAddress(static_cast<HMODULE>(_module.get()), name); }
#elif __unix__
	{  return ::dlsym(_module.get(), name); }
#else
    {  return ::dlsym(_module.get(), name); }
#endif
}
