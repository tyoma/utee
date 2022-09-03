cmake_minimum_required(VERSION 3.13)

add_library(ut INTERFACE)
target_include_directories(ut INTERFACE ..)

add_library(utee STATIC assert.cpp registry.cpp)
target_link_libraries(utee PUBLIC ut)

add_executable(utee-runner module.cpp module.h runner.cpp)
target_link_libraries(utee-runner utee $<$<PLATFORM_ID:Linux>:dl>)
