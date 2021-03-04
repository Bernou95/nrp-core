cmake_minimum_required (VERSION 3.16)

#If not conbine it, when connnect C and C++ ,it need use extern
#SET(CMAKE_C_COMPILER g++) 
set (CMAKE_CXX_STANDARD 20)

project(main)
include_directories(lib)
add_subdirectory(src bin)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

