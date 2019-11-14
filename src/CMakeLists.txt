include_directories(..)

add_compile_options("$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-frtti>")
add_compile_options("$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fPIC>")

add_library(utee STATIC assert.cpp registry.cpp)
add_executable(utee-runner module.cpp module.h runner.cpp)

if(UNIX)
	target_link_libraries(utee-runner dl)
endif()
