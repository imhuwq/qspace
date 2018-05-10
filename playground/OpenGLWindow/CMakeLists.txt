include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# source files
file(GLOB_RECURSE _Play_OpenGLWindow_SOURCES1 "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE _Play_OpenGLWindow_SOURCES2 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
list(APPEND _Play_OpenGLWindow_SOURCES ${_Play_OpenGLWindow_SOURCES1})
list(APPEND _Play_OpenGLWindow_SOURCES ${_Play_OpenGLWindow_SOURCES2})
list(REMOVE_DUPLICATES _Play_OpenGLWindow_SOURCES)

add_executable(play_TriangleWindow main.cpp ${_Play_OpenGLWindow_SOURCES})
target_link_libraries(play_TriangleWindow ${PRJ_LIBRARIES})
