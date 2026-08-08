#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GraphPlot::graph_plot" for configuration ""
set_property(TARGET GraphPlot::graph_plot APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(GraphPlot::graph_plot PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libgraph_plot.1.0.0.dylib"
  IMPORTED_SONAME_NOCONFIG "@rpath/libgraph_plot.1.dylib"
  )

list(APPEND _cmake_import_check_targets GraphPlot::graph_plot )
list(APPEND _cmake_import_check_files_for_GraphPlot::graph_plot "${_IMPORT_PREFIX}/lib/libgraph_plot.1.0.0.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
