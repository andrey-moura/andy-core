cmake_minimum_required(VERSION 3.30)

project(uva-core)

set(CMAKE_CXX_STANDARD 20)

add_library(uva-core
	${CMAKE_CURRENT_LIST_DIR}/src/var.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(uva-core)

install(TARGETS uva-core DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/uva DESTINATION include/uva)