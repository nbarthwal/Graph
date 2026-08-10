cmake_minimum_required(VERSION 3.18)

project(GraphPlotExamples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PREFIX "$ENV{HOME}/.local" CACHE PATH "Package search prefix")

list(APPEND CMAKE_PREFIX_PATH "${PREFIX}")

find_package(GraphPlot REQUIRED)

function(add_graph_plot_example target_name source_file)
    add_executable(${target_name} ${source_file})
    set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "${target_name}.bin")
    target_link_libraries(${target_name} PRIVATE GraphPlot::graph_plot)
endfunction()

add_graph_plot_example(graph_plot_example sine_graph.cpp)
add_graph_plot_example(graph_view_example sine_graph_view.cpp)
add_graph_plot_example(histogram_grouped_example histogram_grouped_example.cpp)
add_graph_plot_example(histogram_slider_example histogram_slider_example.cpp)
add_graph_plot_example(histogram_view_example histogram_view_example.cpp)
add_graph_plot_example(trigonometry trigonometry.cpp)
add_graph_plot_example(fixed_parameter_example fixed_parameter_example.cpp)
