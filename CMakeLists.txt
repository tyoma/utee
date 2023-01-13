cmake_minimum_required(VERSION 3.13)

project(utee)

option(UTEE_NO_TESTS "Do not build test modules." OFF)

if (NOT UTEE_NO_TESTS)
	enable_testing()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/make/build.props)

include(utee-test)

add_compile_options("$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-g;-fPIC>")
add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(src)

if (NOT UTEE_NO_TESTS)
	add_subdirectory(tests)

	add_utee_test(utee.tests)
endif()
