#pragma once

#include "graph_export.h"
#include "histogram.h"

// Opens a Qt window with an embedded histogram plot and slider.
// Parameter p in [MinP(), MaxP()] is controlled by the slider; all data sets are drawn together.
GRAPH_API void PlotHistogram(Histogram& histogram);
