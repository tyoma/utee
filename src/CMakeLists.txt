include_directories(..)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fPIC")

add_library(utee STATIC assert.cpp registry.cpp)
add_executable(utee-runner module.cpp module.h runner.cpp)

if(UNIX)
	target_link_libraries(utee-runner dl)
endif()
