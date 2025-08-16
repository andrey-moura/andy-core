cmake_minimum_required(VERSION 3.10)

project(andy-core)

set(CMAKE_CXX_STANDARD 20)

add_library(andy-core OBJECT
	${CMAKE_CURRENT_LIST_DIR}/src/var.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(andy-core)

# install(TARGETS andy-core EXPORT andy-coreTargets)
# install(EXPORT andy-coreTargets
#     FILE andy-coreTargets.cmake
#     DESTINATION lib/cmake/andy-core
# )
# install(TARGETS andy-core DESTINATION lib)
# install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/andy DESTINATION include)