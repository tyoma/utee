set(UTEE_TEST_SOURCES
	MiscTests.cpp
	TestCaseTests.cpp
	TestExecutionEngineTests.cpp
)

add_library(utee.tests SHARED ${UTEE_TEST_SOURCES})

target_link_libraries(utee.tests utee)
