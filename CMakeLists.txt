#Require a minimum version
cmake_minimum_required(VERSION 3.10)

project(uva-core)

add_library(uva-core
	${CMAKE_CURRENT_LIST_DIR}/src/core.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR})

#Tests

#Create tests for parent project
cspec_configure("uva-core" ${CMAKE_CURRENT_LIST_DIR} "uva-core")