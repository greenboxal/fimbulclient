cmake_minimum_required (VERSION 2.6)
project (FimbulwinterClient)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include (cotire)

include (CheckCXXSourceCompiles)
include (CheckCSourceCompiles)

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message (STATUS "Setting build type to 'Debug' as none was specified.")
  set (CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif ()

# Compile with maximum warnings
if (MSVC)
  if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string (REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif ()

  # Except M$ only deprecations and warnings
  add_definitions (/D_SCL_SECURE_NO_WARNINGS)
  add_definitions (/D_CRT_SECURE_NO_DEPRECATE)
  add_definitions (/D_CRT_NONSTDC_NO_DEPRECATE)
elseif (CMAKE_COMPILER_IS_GNUCC)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Wno-unused-parameter -g -std=c++0x")
endif ()

# Is this a debug build?
if (CMAKE_BUILD_TYPE EQUAL Debug)
  add_definitions (-D_DEBUG)
endif ()

# Boost
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)
find_package (Boost 1.41.0 COMPONENTS system REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})

# GLM
find_package (glm REQUIRED)

# SFML
set (SFML_STATIC_LIBRARIES TRUE)
find_package (SFML 2 COMPONENTS system window graphics REQUIRED)
include_directories (${SFML_INCLUDE_DIR})

# GLEW
find_package (GLEW REQUIRED)
include_directories (${GLEW_INCLUDE_PATH})

# YA3DE
include_directories (src/ya3de)

add_subdirectory (src)
