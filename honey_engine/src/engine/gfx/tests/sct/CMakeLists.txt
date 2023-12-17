if(NOT COVERAGE)
	set(MODULE_NAME_SCT ${MODULE_NAME}_SCT)
	message([Cmake] " add_test_executable " ${MODULE_NAME_SCT} )

	add_test_executable(${MODULE_NAME_SCT}
		TESTS ${MODULE_NAME}
		SOURCES
			ButtonSCT.cpp
			PropagationLayerSCT.cpp
			MenuLayerSCT.cpp
			Model3dSCT.cpp
			RenderFixture.hpp
			ShapeSCT.cpp
			TextSCT.cpp
		LABELS
			${SCT_LABELS}
	)

	target_link_libraries(
		${MODULE_NAME_SCT}
		PRIVATE
			he::render_mocks
		PUBLIC
			glfw3
			GTest::gmock_main
			he::gfx
			he::window
			freetype
	)
endif()