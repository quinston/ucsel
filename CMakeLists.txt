cmake_minimum_required (VERSION 3.0)
project (ucsel)
add_executable (ucsel ${CMAKE_SOURCE_DIR}/src/main.cpp)
set (CMAKE_BUILD_TYPE Release)

find_package (Boost REQUIRED)
include_directories (${BOOST_INCLUDE_DIRS})

#[[
find_program (PYTHON3 python3)
add_custom_command(
 OUTPUT courses.h
 COMMAND PYTHON3
 ARGS makeCourses.py)
]]


find_library(ORTOOLS_LIBRARY ortools lib)
target_link_libraries (ucsel ${ORTOOLS_LIBRARY})


