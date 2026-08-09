cmake_minimum_required(VERSION 3.18)

project(GraphPlot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)

set(INCLUDE_DIR "$ENV{HOME}/.local/include")
set(LIBRARY_DIR "$ENV{HOME}/.local/lib")

option(GRAPH_PLOT_USE_QT6 "Use Qt6 instead of Qt5" ON)

if(GRAPH_PLOT_USE_QT6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    set(QT_WIDGETS_LIB Qt6::Widgets)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_WIDGETS_LIB Qt5::Widgets)
endif()

add_library(graph_plot SHARED
    plot_app.cpp
    graph_canvas.cpp
    graph_view.cpp
    graph_plot.cpp
    histogram_canvas.cpp
    histogram_view.cpp
    histogram_plot.cpp
)

target_compile_definitions(graph_plot PRIVATE GRAPH_PLOT_BUILD_DLL)

target_include_directories(graph_plot
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(graph_plot
    PUBLIC
        ${QT_WIDGETS_LIB}
)

set_target_properties(graph_plot PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME graph_plot
    MACOSX_RPATH ON
    INSTALL_NAME_DIR "${LIBRARY_DIR}"
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${LIBRARY_DIR}"
)

if(APPLE)
    set(GRAPH_PLOT_LIB_FILENAME libgraph_plot.1.dylib)
elseif(WIN32)
    set(GRAPH_PLOT_LIB_FILENAME graph_plot.dll)
else()
    set(GRAPH_PLOT_LIB_FILENAME libgraph_plot.so.1)
endif()

add_custom_command(
    TARGET graph_plot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${INCLUDE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBRARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/graph.h" "${INCLUDE_DIR}/graph.h"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_SONAME_FILE:graph_plot>" "${LIBRARY_DIR}/${GRAPH_PLOT_LIB_FILENAME}"
    COMMAND install_name_tool -id "${LIBRARY_DIR}/${GRAPH_PLOT_LIB_FILENAME}"
        "${LIBRARY_DIR}/${GRAPH_PLOT_LIB_FILENAME}"
    COMMENT "Installing graph.h to ${INCLUDE_DIR} and ${GRAPH_PLOT_LIB_FILENAME} to ${LIBRARY_DIR}"
)
