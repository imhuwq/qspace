cmake_minimum_required(VERSION 3.8)
project(qspace)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif ()

# things to be gathered
set(PRJ_INCLUDE_DIRS "")
set(PRJ_LIBRARIES "")
set(PRJ_SOURCES "")

# packages required
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(ASSIMP)

# include dirs
list(APPEND PRJ_INCLUDE_DIRS includes)
list(APPEND PRJ_INCLUDE_DIRS ${Qt5Core_INCLUDE_DIRS})
list(APPEND PRJ_INCLUDE_DIRS ${Qt5Gui_INCLUDE_DIRS})
list(APPEND PRJ_INCLUDE_DIRS ${Qt5Widgets_INCLUDE_DIRS})
list(APPEND PRJ_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIRS})
list(REMOVE_DUPLICATES PRJ_INCLUDE_DIRS)
include_directories(${PRJ_INCLUDE_DIRS})

# libraries
list(APPEND PRJ_LIBRARIES ${Qt5Core_LIBRARIES})
list(APPEND PRJ_LIBRARIES ${Qt5Gui_LIBRARIES})
list(APPEND PRJ_LIBRARIES ${Qt5Widgets_LIBRARIES})
list(APPEND PRJ_LIBRARIES ${ASSIMP_LIBRARIES})


# source files
file(GLOB_RECURSE PRJ_SOURCES1 src/*.cpp)
file(GLOB_RECURSE PRJ_SOURCES2 includes/*.h)
list(APPEND PRJ_SOURCES ${PRJ_SOURCES1})
list(APPEND PRJ_SOURCES ${PRJ_SOURCES2})
list(REMOVE_DUPLICATES PRJ_SOURCES)

add_executable(run_test test.cpp ${PRJ_SOURCES})
target_link_libraries(run_test ${PRJ_LIBRARIES} gtest_main)
add_test(NAME run_test COMMAND run_test)

add_executable(main main.cpp ${PRJ_SOURCES})
target_link_libraries(main ${PRJ_LIBRARIES})

add_subdirectory(playground)