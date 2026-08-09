cmake_minimum_required(VERSION 3.18)

project(GraphPlot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)

set(PREFIX "$ENV{HOME}/.local")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(QT_WIDGETS_LIB Qt6::Widgets)

add_library(graph_plot STATIC
    plot_app.cpp
    graph_canvas.cpp
    graph_view.cpp
    graph_plot.cpp
    histogram_canvas.cpp
    histogram_view.cpp
    histogram_plot.cpp
)

target_include_directories(graph_plot
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(graph_plot
    PUBLIC
        ${QT_WIDGETS_LIB}
)

set_target_properties(graph_plot PROPERTIES
    OUTPUT_NAME graph_plot
)

if(MSVC)
    set(GRAPH_PLOT_LIB_FILENAME graph_plot.lib)
else()
    set(GRAPH_PLOT_LIB_FILENAME libgraph_plot.a)
endif()

add_custom_command(
    TARGET graph_plot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PREFIX}/include"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PREFIX}/lib"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/graph.h" "${PREFIX}/include/graph.h"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:graph_plot>" "${PREFIX}/lib/${GRAPH_PLOT_LIB_FILENAME}"
    COMMENT "Installing graph.h to ${PREFIX}/include and ${GRAPH_PLOT_LIB_FILENAME} to ${PREFIX}/lib"
)
