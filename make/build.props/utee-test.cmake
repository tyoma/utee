function(add_utee_test test_target)
	target_link_libraries(${test_target} utee)
	add_test(NAME ${test_target} COMMAND $<TARGET_FILE:utee-runner> $<TARGET_FILE:${test_target}> WORKING_DIRECTORY $<TARGET_FILE_DIR:${test_target}>)

	set_target_properties(${test_target} PROPERTIES
		VS_DEBUGGER_COMMAND $<TARGET_FILE:utee-runner>
		VS_DEBUGGER_COMMAND_ARGUMENTS $<TARGET_FILE:${test_target}>
		VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${test_target}>)
endfunction()
