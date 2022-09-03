cmake_minimum_required(VERSION 3.13)

set(UTEE_TEST_SOURCES
	TestCaseTests.cpp
	TestExecutionEngineTests.cpp
)

add_library(utee.tests SHARED ${UTEE_TEST_SOURCES})

target_link_libraries(utee.tests utee)
