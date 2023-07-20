set(MODULE_NAME_UT ${MODULE_NAME}_UT)
	# message([Cmake] " add_test_executable " ${TEST_NAME2} )

add_test_executable(${MODULE_NAME_UT}
	TESTS ${MODULE_NAME}
	SOURCES
		FiguresTests.cpp
		Point2dTests.cpp
		Transformable2dTests.cpp
	LABELS
		${UT_LABELS}
)

target_link_libraries(
	${MODULE_NAME_UT}
	gtest_main
	gfx::geometry
)
