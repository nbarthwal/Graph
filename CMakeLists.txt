cmake_minimum_required(VERSION 3.18)

project(GraphPlot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GRAPH_PLOT_BUILD_EXAMPLES "Build example executables" ON)

add_subdirectory(Library)

if(GRAPH_PLOT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
