cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Global properties
# set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project name
project(engine2d)


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES src/*.cpp)

if(NOT ENGINE_MODULE)
    add_executable(engine2d ${SOURCES})
else()
    add_library(engine2d ${SOURCES})
endif()