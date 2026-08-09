cmake_minimum_required(VERSION 3.18)

project(GraphPlotExamples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(INCLUDE_DIR "$ENV{HOME}/.local/include")
set(LIBRARY_DIR "$ENV{HOME}/.local/lib")

set(CMAKE_BUILD_RPATH "${LIBRARY_DIR}")
set(CMAKE_INSTALL_RPATH "${LIBRARY_DIR}")

option(GRAPH_PLOT_USE_QT6 "Use Qt6 instead of Qt5" ON)

if(GRAPH_PLOT_USE_QT6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    set(QT_WIDGETS_LIB Qt6::Widgets)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_WIDGETS_LIB Qt5::Widgets)
endif()

if(APPLE)
    set(GRAPH_PLOT_LIB "${LIBRARY_DIR}/libgraph_plot.1.dylib")
elseif(WIN32)
    set(GRAPH_PLOT_LIB "${LIBRARY_DIR}/graph_plot.dll")
else()
    set(GRAPH_PLOT_LIB "${LIBRARY_DIR}/libgraph_plot.so.1")
endif()

function(add_graph_plot_example target_name source_file)
    add_executable(${target_name} ${source_file})
    target_include_directories(${target_name} PRIVATE "${INCLUDE_DIR}")
    target_link_libraries(${target_name} PRIVATE "${GRAPH_PLOT_LIB}" ${QT_WIDGETS_LIB})
    set_target_properties(${target_name} PROPERTIES
        BUILD_RPATH "${LIBRARY_DIR}"
        INSTALL_RPATH "${LIBRARY_DIR}"
    )
endfunction()

add_graph_plot_example(graph_plot_example sine_graph.cpp)
add_graph_plot_example(graph_view_example sine_graph_view.cpp)
add_graph_plot_example(histogram_grouped_example histogram_grouped_example.cpp)
add_graph_plot_example(histogram_slider_example histogram_slider_example.cpp)
add_graph_plot_example(histogram_view_example histogram_view_example.cpp)
add_graph_plot_example(trigonometry trigonometry.cpp)
