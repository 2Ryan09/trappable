
#ifndef STABILITYREGIONPLOTTER_H
#define STABILITYREGIONPLOTTER_H

#include "QCustomPlot/qcustomplot.h"

class StabilityRegionPlotter {
   public:
    StabilityRegionPlotter(QCustomPlot* plot);
    void setupStabilityRegion(QCustomPlot* customPlot);
    void plotPoint(double q, double a);

   private:
    double calculateUpperBoundary(double q);
    QCustomPlot* m_plot;
    QCPGraph* m_pointGraph;
};

#endif  // STABILITYREGIONPLOTTER_H
