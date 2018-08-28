cmake_minimum_required(VERSION 3.8)
project(qspace)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS " -pthread ")
set(CMAKE_AUTOMOC ON)

# things to be gathered
set(PRJ_INCLUDE_DIRS "")
set(PRJ_LIBRARIES "")
set(PRJ_SOURCES "")

# packages required
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

# include dirs
list(APPEND PRJ_INCLUDE_DIRS include)
list(APPEND PRJ_INCLUDE_DIRS lib)
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
list(APPEND PRJ_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/fbxsdk/libfbxsdk.a")

# source files
file(GLOB_RECURSE PRJ_SOURCES1 src/*.cpp)
file(GLOB_RECURSE PRJ_SOURCES2 include/*.h)
list(APPEND PRJ_SOURCES ${PRJ_SOURCES1})
list(APPEND PRJ_SOURCES ${PRJ_SOURCES2})
list(REMOVE_DUPLICATES PRJ_SOURCES)

add_executable(main main.cpp ${PRJ_SOURCES})
target_link_libraries(main ${PRJ_LIBRARIES} ${CMAKE_DL_LIBS})
