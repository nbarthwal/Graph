#pragma once

#include "graph.h"
#include "graph_export.h"

// Opens an interactive plot window for the given graph.
// A Qt slider controls parameter p in [MinP(), MaxP()]; all curves are redrawn on change.
GRAPH_API void Plot(Graph& graph);
