

#include "StabilityRegionPlotter.h"

#include <QVector>
#include <QtMath>

#include "QCustomPlot/qcustomplot.h"
#include "stability/StabilityCalculator.h"

StabilityRegionPlotter::StabilityRegionPlotter(QCustomPlot* plot)
    : m_plot(plot), m_pointGraph(nullptr) {
    setupStabilityRegion(plot);
}

void StabilityRegionPlotter::setupStabilityRegion(QCustomPlot* customPlot) {
    customPlot->clearPlottables();
    QCPCurve* stabilityRegion = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    QVector<double> q_values, a_values;
    int numPoints = 500;
    for (int i = 0; i <= numPoints; ++i) {
        double q = 0.908 * i / numPoints;
        double a = StabilityCalculator::calculateUpperBoundary(q);
        q_values.append(q);
        a_values.append(a);
    }
    for (int i = numPoints; i >= 0; --i) {
        double q = 0.908 * i / numPoints;
        q_values.append(q);
        a_values.append(0.0);
    }
    stabilityRegion->setData(q_values, a_values);
    QPen regionPen(Qt::blue);
    regionPen.setWidth(2);
    stabilityRegion->setPen(regionPen);
    QBrush regionBrush(QColor(0, 100, 255, 80));
    stabilityRegion->setBrush(regionBrush);
    stabilityRegion->setLineStyle(QCPCurve::lsLine);
    customPlot->xAxis->setLabel("Mathieu q");
    customPlot->yAxis->setLabel("Mathieu a");
    customPlot->xAxis->setRange(0, 0.908);
    customPlot->yAxis->setRange(0, 0.25);
    customPlot->legend->setVisible(false);
    customPlot->axisRect()->setMargins(QMargins(40, 20, 20, 40));
    customPlot->replot();
}

// Removed: now using StabilityCalculator::calculateUpperBoundary

void StabilityRegionPlotter::plotPoint(double q, double a) {
    if (!m_plot)
        return;
    // Remove previous point graph if it exists
    if (m_pointGraph) {
        m_plot->removeGraph(m_pointGraph);
        m_pointGraph = nullptr;
    }
    // Add new point graph
    m_pointGraph = m_plot->addGraph();
    m_pointGraph->setLineStyle(QCPGraph::lsNone);
    m_pointGraph->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::red), QBrush(Qt::red), 16));
    m_pointGraph->addData(q, a);
    m_plot->replot();
}

void StabilityRegionPlotter::drawVerticalDistance(double q, double a, double a_boundary) {
    if (!m_plot)
        return;
    // Remove previous vertical line if it exists
    if (m_verticalLine) {
        m_plot->removeItem(m_verticalLine);
        m_verticalLine = nullptr;
    }
    m_verticalLine = new QCPItemLine(m_plot);
    m_verticalLine->setObjectName("verticalDistanceLine");
    m_verticalLine->start->setType(QCPItemPosition::ptPlotCoords);
    m_verticalLine->end->setType(QCPItemPosition::ptPlotCoords);
    m_verticalLine->start->setCoords(q, a);
    m_verticalLine->end->setCoords(q, a_boundary);
    QPen pen(Qt::darkMagenta);
    pen.setWidth(3);
    m_verticalLine->setPen(pen);
    m_plot->replot();
}
