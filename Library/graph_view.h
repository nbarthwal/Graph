#pragma once

#include "graph.h"
#include "graph_export.h"

// Opens a Qt window with an embedded graph plot at a fixed parameter value.
GRAPH_API void ShowGraph(Graph& graph, float parameter);
