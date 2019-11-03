include_directories(..)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fPIC")
endif()

add_library(utee STATIC assert.cpp registry.cpp)
add_executable(utee-runner module.cpp module.h runner.cpp)

if(UNIX)
	target_link_libraries(utee-runner dl)
endif()
