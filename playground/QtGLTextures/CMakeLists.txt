include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# source files
file(GLOB_RECURSE _Play_QtGLTextures_SOURCES1 "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE _Play_QtGLTextures_SOURCES2 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
list(APPEND _Play_QtGLTextures_SOURCES ${_Play_QtGLTextures_SOURCES1})
list(APPEND _Play_QtGLTextures_SOURCES ${_Play_QtGLTextures_SOURCES2})
list(REMOVE_DUPLICATES _Play_QtGLTextures_SOURCES)

add_executable(play_QtGLTextures main.cpp ${_Play_QtGLTextures_SOURCES})
target_link_libraries(play_QtGLTextures ${PRJ_LIBRARIES})
