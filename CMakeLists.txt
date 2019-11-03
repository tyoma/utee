cmake_minimum_required(VERSION 2.8)

project(UTEE)

include_directories(${UTEE_SOURCE_DIR})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fPIC")
endif()

add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(src)
add_subdirectory(tests)
