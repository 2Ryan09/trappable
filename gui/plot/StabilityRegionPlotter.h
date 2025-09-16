
#ifndef STABILITYREGIONPLOTTER_H
#define STABILITYREGIONPLOTTER_H

#include "QCustomPlot/qcustomplot.h"

class StabilityRegionPlotter {
   public:
    StabilityRegionPlotter(QCustomPlot* plot);
    void setupStabilityRegion(QCustomPlot* customPlot);

   private:
    double calculateUpperBoundary(double q);
};

#endif  // STABILITYREGIONPLOTTER_H
