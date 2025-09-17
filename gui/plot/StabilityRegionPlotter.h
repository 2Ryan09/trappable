
#ifndef STABILITYREGIONPLOTTER_H
#define STABILITYREGIONPLOTTER_H

#include "QCustomPlot/qcustomplot.h"

class StabilityRegionPlotter {
   public:
    StabilityRegionPlotter(QCustomPlot* plot);
    void setupStabilityRegion(QCustomPlot* customPlot);
    void plotPoint(double q, double a);
    void clearPlot();
    void drawNearestPointTriangle(double q, double a, double q_b, double a_b);
    void drawUnstablePoint(double q, double a);
    double calculateUpperBoundary(double q);

   private:
    QCustomPlot* m_plot;
    QCPGraph* m_pointGraph;
    QCPItemLine* m_verticalLine = nullptr;
    QCPItemLine* m_leftHorizontalLine = nullptr;
    QCPItemLine* m_rightHorizontalLine = nullptr;
    QCPItemLine* m_xLine1 = nullptr;
    QCPItemLine* m_xLine2 = nullptr;
};

#endif  // STABILITYREGIONPLOTTER_H
