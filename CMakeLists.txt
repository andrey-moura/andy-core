project(uva-core)

add_library(uva-core
	${CMAKE_CURRENT_LIST_DIR}/src/var.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(uva-core)