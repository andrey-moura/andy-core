cmake_minimum_required(VERSION 3.10)

project(uva-core)

set(CMAKE_CXX_STANDARD 20)

add_library(uva-core OBJECT
	${CMAKE_CURRENT_LIST_DIR}/src/var.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(uva-core)

# install(TARGETS uva-core EXPORT uva-coreTargets)
# install(EXPORT uva-coreTargets
#     FILE uva-coreTargets.cmake
#     DESTINATION lib/cmake/uva-core
# )
# install(TARGETS uva-core DESTINATION lib)
# install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/uva DESTINATION include)