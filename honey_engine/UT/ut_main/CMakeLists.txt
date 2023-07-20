set(MODULE_NAME ut_main)

# add_library(ut_main ut_main.cpp)

add_test_executable(ut_main
	TESTS ${MODULE_NAME}
	SOURCES
		ut_main.cpp
	LABELS
		${UT_LABELS}
)

target_link_libraries(
	ut_main
    gmock_main
	gtest_main
)
