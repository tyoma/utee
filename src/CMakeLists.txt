include_directories(..)

add_library(utee STATIC assert.cpp registry.cpp)
add_executable(utee-runner module.cpp module.h runner.cpp)

target_link_libraries(utee-runner $<$<PLATFORM_ID:Linux>:dl>)
