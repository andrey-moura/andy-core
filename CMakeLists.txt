#Require a minimum version
cmake_minimum_required(VERSION 3.10)

project(uva-core)

add_library(uva-core
	${CMAKE_CURRENT_LIST_DIR}/src/core.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(uva-core uva-string)

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
	if(NOT UVA_DEBUG_LEVEL)
    	set(UVA_DEBUG_LEVEL 1)
	endif()
endif (CMAKE_BUILD_TYPE EQUAL "DEBUG")

#Tests

#Create tests for parent project
cspec_configure("uva-core" ${CMAKE_CURRENT_LIST_DIR} "uva-core")