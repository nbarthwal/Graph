cmake_minimum_required(VERSION 3.18)

project(GraphPlotExamples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(INCLUDE_DIR "$ENV{HOME}/.local/include")
set(LIBRARY_DIR "$ENV{HOME}/.local/lib")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(QT_WIDGETS_LIB Qt6::Widgets)

if(MSVC)
    set(GRAPH_PLOT_LIB "${LIBRARY_DIR}/graph_plot.lib")
else()
    set(GRAPH_PLOT_LIB "${LIBRARY_DIR}/libgraph_plot.a")
endif()

function(add_graph_plot_example target_name source_file)
    add_executable(${target_name} ${source_file})
    target_include_directories(${target_name} PRIVATE "${INCLUDE_DIR}")
    target_link_libraries(${target_name} PRIVATE "${GRAPH_PLOT_LIB}" ${QT_WIDGETS_LIB})
endfunction()

add_graph_plot_example(graph_plot_example sine_graph.cpp)
add_graph_plot_example(graph_view_example sine_graph_view.cpp)
add_graph_plot_example(histogram_grouped_example histogram_grouped_example.cpp)
add_graph_plot_example(histogram_slider_example histogram_slider_example.cpp)
add_graph_plot_example(histogram_view_example histogram_view_example.cpp)
add_graph_plot_example(trigonometry trigonometry.cpp)
