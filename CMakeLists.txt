project(uva-core)

add_library(uva-core
	${CMAKE_CURRENT_LIST_DIR}/src/core.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/var.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(uva-core)

#Tests
if(TARGET cspec)
	#Create tests for project
	cspec_configure("uva-core" ${CMAKE_CURRENT_LIST_DIR} "uva-core")
endif()