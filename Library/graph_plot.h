#pragma once

#include "graph.h"
#include "graph_export.h"

// Opens a Qt window with an embedded plot and slider.
// Parameter p in [MinP(), MaxP()] is controlled by the slider; all curves overlap in one plot.
GRAPH_API void Plot(Graph& graph);
