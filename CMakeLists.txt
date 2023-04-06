project(uva-core)

add_library(uva-core
	${CMAKE_CURRENT_LIST_DIR}/core.hpp
	${CMAKE_CURRENT_LIST_DIR}/color.hpp
	${CMAKE_CURRENT_LIST_DIR}/src/core.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(uva-core uva-string)

#Tests

#Create tests for parent project
cspec_configure("uva-core" ${CMAKE_CURRENT_LIST_DIR} "uva-core")