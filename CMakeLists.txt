#Require a minimum version
cmake_minimum_required(VERSION 3.16)

project(uva-core)

add_library(uva-core
	${CMAKE_CURRENT_LIST_DIR}/src/core.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(uva-core uva-string)

#Tests

#Create tests for parent project
cspec_configure("uva-core" ${CMAKE_CURRENT_LIST_DIR} "uva-core")