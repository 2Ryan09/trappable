
#ifndef STABILITYREGIONPLOTTER_H
#define STABILITYREGIONPLOTTER_H

#include "QCustomPlot/qcustomplot.h"

class StabilityRegionPlotter {
   public:
    StabilityRegionPlotter(QCustomPlot* plot);
    void setupStabilityRegion(QCustomPlot* customPlot);
    void plotPoint(double q, double a);
    double drawVerticalDistanceIndicator(double q, double a, double a_boundary);
    double drawLeftHorizontalDistanceIndicator(double q, double a);
    double drawRightHorizontalDistanceIndicator(double q, double a);
    std::tuple<double, double, double> drawDistanceIndicators(double q, double a,
                                                              double a_boundary);
    double calculateUpperBoundary(double q);

   private:
    QCustomPlot* m_plot;
    QCPGraph* m_pointGraph;
    QCPItemLine* m_verticalLine = nullptr;
    QCPItemLine* m_leftHorizontalLine = nullptr;
    QCPItemLine* m_rightHorizontalLine = nullptr;
};

#endif  // STABILITYREGIONPLOTTER_H
