cmake_minimum_required (VERSION 3.0)

message (STATUS ${ucsel_SOURCE_DIR})

include_directories(${ucsel_SOURCE_DIR}/src)

add_executable (ucsel main.cpp)
set_target_properties(ucsel PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ucsel_SOURCE_DIR}/bin)

message (STATUS "Build type: ${CMAKE_BUILD_TYPE}")
add_definitions(-std=c++11 -Wall)

find_program (PYTHON3 python3)

message(STATUS ${ucsel_SOURCE_DIR}/lib)
find_library(ORTOOLS_LIBRARY ortools PATHS ${ucsel_SOURCE_DIR}/lib)

include_directories(${ucsel_SOURCE_DIR}/include)

target_link_libraries (ucsel ${ORTOOLS_LIBRARY})


find_package(Guile 2.0 REQUIRED)
include_directories(ucsel ${GUILE_INCLUDE_DIRS})
target_link_libraries(ucsel ${GUILE_LIBRARIES})
