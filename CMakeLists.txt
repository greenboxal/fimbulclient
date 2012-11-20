cmake_minimum_required (VERSION 2.6)
project (FimbulwinterClient)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include (cotire)

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message (STATUS "Setting build type to 'Debug' as none was specified.")
  set (CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif ()

if (MSVC)
  add_definitions (-D_SCL_SECURE_NO_WARNINGS)
  add_definitions (-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions (-D_CRT_NONSTDC_NO_DEPRECATE)
  add_definitions (-DWIN32_LEAN_AND_MEAN)
elseif (CMAKE_COMPILER_IS_GNUCC)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x")
endif ()

# Is this a debug build?
if (CMAKE_BUILD_TYPE EQUAL Debug)
  add_definitions (-D_DEBUG)
endif ()

# Defines
set (SOIL_BUILTIN_BUILD ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON CACHE STRING "Use boost as static library")
set (SFML_STATIC_LIBRARIES TRUE CACHE STRING "Use SFML as a static library")

# Packages
find_package (Boost 1.41.0 COMPONENTS system REQUIRED)
find_package (GLM REQUIRED)
find_package (SFML 2 COMPONENTS system window graphics REQUIRED)
find_package (GLEW REQUIRED)
find_package (Awesomium REQUIRED)

# Includes
include_directories (${Boost_INCLUDE_DIRS})
include_directories (${GLM_INCLUDE_DIR})
include_directories (${SFML_INCLUDE_DIR})
include_directories (${GLEW_INCLUDE_PATH})
include_directories (${AWESOMIUM_INCLUDE_DIR})
include_directories (3rdparty/SOIL/src)
include_directories (src/ya3de)

add_subdirectory (3rdparty/SOIL)
add_subdirectory (src)
