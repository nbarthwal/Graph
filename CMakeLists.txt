cmake_minimum_required(VERSION 3.18)

project(GraphPlot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_AUTOMOC ON)

option(GRAPH_PLOT_USE_QT6 "Use Qt6 instead of Qt5" ON)

if(GRAPH_PLOT_USE_QT6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    set(QT_WIDGETS_LIB Qt6::Widgets)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_WIDGETS_LIB Qt5::Widgets)
endif()

add_library(graph_plot SHARED
    graph_plot.cpp
    histogram_plot.cpp
)

target_compile_definitions(graph_plot PRIVATE GRAPH_PLOT_BUILD_DLL)

target_include_directories(graph_plot
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(graph_plot
    PUBLIC
        ${QT_WIDGETS_LIB}
)

set_target_properties(graph_plot PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME graph_plot
)

install(TARGETS graph_plot
    EXPORT GraphPlotTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    FILES
        graph.h
        graph_plot.h
        histogram.h
        histogram_plot.h
        graph_export.h
    DESTINATION include
)

install(EXPORT GraphPlotTargets
    FILE GraphPlotTargets.cmake
    NAMESPACE GraphPlot::
    DESTINATION lib/cmake/GraphPlot
)

option(GRAPH_PLOT_BUILD_EXAMPLES "Build example executables" ON)
if(GRAPH_PLOT_BUILD_EXAMPLES)
    add_executable(graph_plot_example examples/sine_graph.cpp)
    target_link_libraries(graph_plot_example PRIVATE graph_plot)

    add_executable(histogram_grouped_example examples/histogram_grouped_example.cpp)
    target_link_libraries(histogram_grouped_example PRIVATE graph_plot)

    add_executable(histogram_slider_example examples/histogram_slider_example.cpp)
    target_link_libraries(histogram_slider_example PRIVATE graph_plot)
endif()
