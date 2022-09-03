function(add_utee_test test_target)
	target_link_libraries(${test_target} utee)
	add_test(NAME ${test_target} COMMAND $<TARGET_FILE:utee-runner> $<TARGET_FILE:${test_target}> WORKING_DIRECTORY $<TARGET_FILE_DIR:${test_target}>)
endfunction()
