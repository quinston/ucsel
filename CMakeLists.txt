cmake_minimum_required (VERSION 3.0)
project(ucsel)
subdirs (src)
subdirs (py)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
