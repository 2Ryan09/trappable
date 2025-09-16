
#ifndef STABILITYREGIONPLOTTER_H
#define STABILITYREGIONPLOTTER_H

#include "QCustomPlot/qcustomplot.h"

class StabilityRegionPlotter {
   public:
    StabilityRegionPlotter(QCustomPlot* plot);
    void setupStabilityRegion(QCustomPlot* customPlot);
    void plotPoint(double q, double a);
    void drawVerticalDistance(double q, double a, double a_boundary);
    double calculateUpperBoundary(double q);

   private:
    QCustomPlot* m_plot;
    QCPGraph* m_pointGraph;
    QCPItemLine* m_verticalLine = nullptr;
};

#endif  // STABILITYREGIONPLOTTER_H
