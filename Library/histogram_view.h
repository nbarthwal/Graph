#pragma once

#include "graph_export.h"
#include "histogram.h"

// Opens a Qt window with an embedded histogram plot at a fixed parameter value.
GRAPH_API void ShowHistogram(Histogram& histogram, float parameter);
