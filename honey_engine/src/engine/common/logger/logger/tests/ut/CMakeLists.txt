set(MODULE_NAME_UT ${MODULE_NAME}_UT)
	# message([Cmake] " add_test_executable " ${TEST_NAME2} )

add_test_executable(${MODULE_NAME_UT}
	TESTS ${MODULE_NAME}
	SOURCES
		LoggerTests.cpp
	LABELS
		${UT_LABELS}
)

target_link_libraries(
	${MODULE_NAME_UT}
	PUBLIC
		gtest_main
		gmock_main
	PRIVATE
		logger
)
