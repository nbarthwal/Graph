#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
#  ifdef GRAPH_PLOT_BUILD_DLL
#    define GRAPH_API __declspec(dllexport)
#  else
#    define GRAPH_API __declspec(dllimport)
#  endif
#else
#  define GRAPH_API __attribute__((visibility("default")))
#endif
