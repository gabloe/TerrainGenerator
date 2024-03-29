# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (terrain-generator)


# set(BUILD_SHARED_LIBS ON)
IF (WIN32)
ELSE()
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration" CACHE STRING "CMAKE C flags")
ENDIF()
add_subdirectory(lib/freetype EXCLUDE_FROM_ALL)

#
# OpenGL is required.
#
find_package (OpenGL REQUIRED)

if (CMAKE_VERSION VERSION_LESS 3.10)
  find_library(OPENGL_egl_LIBRARY NAMES EGL)
  if (OPENGL_egl_LIBRARY)
    set (OpenGL_EGL_FOUND TRUE)
  endif ()
endif ()

if (OPENGL_opengl_LIBRARY)
  set (OUR_LIBRARIES ${OPENGL_opengl_LIBRARY})
else ()
  set (OUR_LIBRARIES ${OPENGL_gl_LIBRARY})
endif ()

if (GLEW_X11 AND NOT WIN32 AND NOT APPLE)
  find_package (X11)
  list (APPEND OUR_LIBRARIES ${OPENGL_glx_LIBRARY} ${X11_LIBRARIES})
endif()

if (GLEW_EGL AND UNIX)
  add_definitions (-DGLEW_EGL)
  if (NOT OpenGL_EGL_FOUND)
    message (FATAL_ERROR "EGL library set but not found.")
  endif ()
  list (APPEND OUR_LIBRARIES ${OPENGL_egl_LIBRARY})
endif ()


#includes
include_directories(src/Includes lib/stb lib/json/single_include lib/freetype/include)

file(GLOB terrain-generator-code
     "src/**/*.hpp"
     "src/*.cpp"
     "src/**/*.cpp"
)

# The main executable
add_executable(terrain-generator
  ${terrain-generator-code}
)

set_property(TARGET terrain-generator PROPERTY CXX_STANDARD 17)

IF (WIN32)
ELSE()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
ENDIF()

target_compile_options(terrain-generator PRIVATE) #-Wall)

add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)

# Disable all exporters
OPTION(ASSIMP_NO_EXPORT "disable exporters" TRUE)

# Limit importers
set(ASSIMP_IMPORTER_ENABLED TRUE)
OPTION(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT "Do not build all importers by default" FALSE)
OPTION(ASSIMP_BUILD_COLLADA_IMPORTER "Build the Collada importers" TRUE)
add_subdirectory(lib/assimp EXCLUDE_FROM_ALL)

add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
add_subdirectory(lib/glad EXCLUDE_FROM_ALL)
add_subdirectory(lib/json EXCLUDE_FROM_ALL)

target_link_libraries(terrain-generator
  PRIVATE glfw
  PRIVATE glm
  PRIVATE assimp
  PRIVATE glad
  PRIVATE freetype
  ${OUR_LIBRARIES}
)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
)
add_dependencies(terrain-generator copy_assets)
