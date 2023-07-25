cmake_minimum_required(VERSION 3.25)

project(honey_engine)


#=================================================================
#	SET

set(EXECUTABLE_NAME "honey_engine")

message([Cmake] " CURRENT DIR= " ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCES
	src/engine/gfx/src/draw/Shape.cpp
	src/engine/window/src/events/EventManager.cpp
	src/engine/window/src/window/Window.cpp
	src/main/src/Main.cpp
)
add_executable(${EXECUTABLE_NAME} ${SOURCES})

set(TEST_NAME_UT "" CACHE STRING "")

set(BUILD_UT $ENV{BUILD_UT})
set(COVERAGE $ENV{COVERAGE})

set(GLEW_LIB OFF)
set(FREEGLUT_LIB OFF)
set(GLFW_LIB ON)
set(FREETYPE_LIB ON)

set( CMAKE_BUILD_TYPE Debug)
set( CMAKE_CXX_STANDARD 20)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")


#=================================================================
#	FUNCTIONS

function(setup_tests_label LABEL)
	add_custom_target(${LABEL}_build)
endfunction()

function(add_ut_subdirectory)
	if(BUILD_UT)
		if(${ARGC} EQUAL 0)
			set(UT_DIR "ut")
		elseif(${ARGC} EQUAL 1)
			set(UT_DIR "${ARGV0}")
		else()
			message(FATAL_ERROR "Too many arguments provided. 0 is supported")
		endif()
	add_subdirectory("${UT_DIR}")
	endif()
endfunction()

function(add_test_executable TEST_NAME)
	set(options)
	set(oneValueArgs TESTS)
	set(multiValueArgs SOURCES DEPENDS LABELS)
	cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
	add_executable(${TEST_NAME} "${ARGS_SOURCES}")
endfunction()


#=================================================================
#	SUBDIRECTORIES

add_subdirectory(src)


#=================================================================
#	LIBS

if(BUILD_UT)
	message([Cmake] " BUILD_UT= " ${BUILD_UT})

	add_subdirectory(UT)
	add_subdirectory(libs/googletest)
	add_subdirectory(libs/glfw-3.3.8)

	enable_testing()
	include(GoogleTest)
	setup_tests_label(ut)
	set(UT_LABELS ut)
endif()

if(GLEW_LIB)
	message([Cmake] " GLEW_LIB= " ${GLEW_LIB})

	# set( GLEW_DEST "D:/Projects/CPP/games/honey_engine/honey_engine/libs/glew-2.1.0")
	# set( GLEW_LIBS glew32 )
	# set( GLEW_STATIC TRUE )
	# set( GLEWAPI TRUE )
endif()

if(FREEGLUT_LIB)
	message([Cmake] " FREEGLUT_LIB= " ${FREEGLUT_LIB})

	set( FREEGLUT_LIBS freeglut )
	target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${FREEGLUT_LIBS})
	list(APPEND CXX_FLAGS "-LD:/Projects/CPP/games/honey_engine/honey_engine/libs/freeglut-3.4.0/freeglut/lib")
endif()

if(GLFW_LIB)
	message([Cmake] " GLFW_LIB= " ${GLFW_LIB})

	set( GLFW_LIBS glfw3 )
	target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${GLFW_LIBS})
	# note: place into mingw folders /include and /lib also ( C:\mingw64_posix\x86_64-w64-mingw32\include )
	# list(APPEND CXX_FLAGS "-LD:/Projects/CPP/games/honey_engine/honey_engine/libs/glfw-3.3.8/glfw/lib")
endif()

if(FREETYPE_LIB)
	message([Cmake] " FREETYPE_LIB= " ${FREETYPE_LIB})

	# set( FREETYPE_LIBRARY 				"D:/Projects/CPP/games/honey_engine/honey_engine/libs/freetype-2.12.1/freetype/lib" )
	# set( FREETYPE_INCLUDE_DIRS 			"D:/Projects/CPP/games/honey_engine/honey_engine/libs/freetype-2.12.1/freetype/include" )
	# set( FREETYPE_INCLUDE_DIR_ft2build 	"D:/Projects/CPP/games/honey_engine/honey_engine/libs/freetype-2.12.1/freetype/include/freetype2" )
	# set ( FREETYPE_DIR "D:/Projects/CPP/games/honey_engine/honey_engine/libs/freetype-2.12.1/freetype" )
	# find_package(Freetype REQUIRED)

	set( FREETYPE_LIBS freetype )
	target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${FREETYPE_LIBS})
	# target_include_directories(${EXECUTABLE_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})

endif()


#=================================================================
#	END
# list(APPEND CXX_DIR "-ID:/Projects/CPP/games/honey_engine/honey_engine/libs/freetype-2.12.1/freetype/include")
# list(APPEND CXX_DIR "-ID:/Projects/CPP/games/honey_engine/honey_engine/libs/freeglut-3.4.0/freeglut/include")
# target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CXX_DIR})

list( APPEND CXX_FLAGS "-lglut32")
list( APPEND CXX_FLAGS "-lglfw3")
list( APPEND CXX_FLAGS "-lfreeglut" "-lopengl32" )
list( APPEND CXX_FLAGS "-lgdi32")
list( APPEND CXX_FLAGS "-lfreetype")

list(APPEND CXX_FLAGS "-Wunused-function" "-Wunused-label" "-Wunused-value" "-Wunused-variable" "-Wunused-const-variable=1")
list(APPEND CXX_FLAGS "-Wswitch-default" "-Wswitch-enum" "-Wextra" "-Werror" "-Wall")
list(APPEND CXX_FLAGS "-Wno-old-style-cast" )
list(APPEND CXX_FLAGS "-pthread" "-fasynchronous-unwind-tables" "-std=c++20" "-march=native")
# list(APPEND CXX_FLAGS_DEBUG "-Wsuggest-final-types" "-Wsuggest-final-methods" "-Wsuggest-override")
list(APPEND CXX_FLAGS_RELEASE "-O3")
list(APPEND CXX_FLAGS_DEBUG "-g" "-O0")

# disable second console after .exe starts
list(APPEND CXX_FLAGS "-mwindows")

# gcov options
if(COVERAGE)
	target_link_libraries(${EXECUTABLE_NAME} PUBLIC gcov)
	list(APPEND CXX_FLAGS "--coverage")
	list(APPEND CXX_FLAGS "-fprofile-arcs" "-ftest-coverage")
endif()

target_compile_options(${EXECUTABLE_NAME} PUBLIC ${CXX_FLAGS})
