set(UTEE_TEST_SOURCES
	main.cpp
	MiscTests.cpp
	TestCaseTests.cpp
	TestExecutionEngineTests.cpp
)

add_executable(utee.tests ${UTEE_TEST_SOURCES})

target_link_libraries(utee.tests utee)
