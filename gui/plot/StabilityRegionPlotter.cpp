
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

void StabilityRegionPlotter::drawNearestPointTriangle(double q, double a, double q_b, double a_b) {
    if (!m_plot)
        return;
    // Remove previous lines if they exist
    if (m_verticalLine) {
        m_plot->removeItem(m_verticalLine);
        m_verticalLine = nullptr;
    }
    if (m_leftHorizontalLine) {
        m_plot->removeItem(m_leftHorizontalLine);
        m_leftHorizontalLine = nullptr;
    }
    if (m_rightHorizontalLine) {
        m_plot->removeItem(m_rightHorizontalLine);
        m_rightHorizontalLine = nullptr;
    }
    // Draw horizontal line from (q, a) to (q_b, a)
    m_leftHorizontalLine = new QCPItemLine(m_plot);
    m_leftHorizontalLine->setObjectName("horizontalDistanceLine");
    m_leftHorizontalLine->start->setType(QCPItemPosition::ptPlotCoords);
    m_leftHorizontalLine->end->setType(QCPItemPosition::ptPlotCoords);
    m_leftHorizontalLine->start->setCoords(q, a);
    m_leftHorizontalLine->end->setCoords(q_b, a);
    QPen hPen(Qt::darkCyan);
    hPen.setWidth(2);
    m_leftHorizontalLine->setPen(hPen);

    // Draw vertical line from (q_b, a) to (q_b, a_b)
    m_verticalLine = new QCPItemLine(m_plot);
    m_verticalLine->setObjectName("verticalDistanceLine");
    m_verticalLine->start->setType(QCPItemPosition::ptPlotCoords);
    m_verticalLine->end->setType(QCPItemPosition::ptPlotCoords);
    m_verticalLine->start->setCoords(q_b, a);
    m_verticalLine->end->setCoords(q_b, a_b);
    QPen pen(Qt::darkMagenta);
    pen.setWidth(3);
    m_verticalLine->setPen(pen);

    // Draw hypotenuse from (q, a) to (q_b, a_b)
    m_rightHorizontalLine = new QCPItemLine(m_plot);
    m_rightHorizontalLine->setObjectName("euclideanDistanceLine");
    m_rightHorizontalLine->start->setType(QCPItemPosition::ptPlotCoords);
    m_rightHorizontalLine->end->setType(QCPItemPosition::ptPlotCoords);
    m_rightHorizontalLine->start->setCoords(q, a);
    m_rightHorizontalLine->end->setCoords(q_b, a_b);
    QPen ePen(Qt::darkYellow);
    ePen.setWidth(2);
    m_rightHorizontalLine->setPen(ePen);

    m_plot->replot();
}
