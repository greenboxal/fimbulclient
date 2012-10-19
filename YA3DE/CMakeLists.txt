cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(YA3DE)

SET(CMAKE_BUILD_TYPE Debug CACHE STRING "default to debug" FORCE)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-D_DEBUG)
endif()

if(WIN32)
    add_definitions(-D_WIN32)
else(CMAKE_SYSTEM_NAME STREQUAL Linux)
    add_definitions(-D_LINUX)
else()
    message(FATAL_ERROR "Unsupported platform") 
endif()

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(YA3DE)

