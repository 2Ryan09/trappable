
#include "StabilityRegionPlotter.h"

#include <QVector>
#include <QtMath>

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
        double a = calculateUpperBoundary(q);
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

double StabilityRegionPlotter::calculateUpperBoundary(double q) {
    if (q <= 0.4) {
        return (q * q / 2.0) - (7.0 * qPow(q, 4) / 128.0);
    } else if (q <= 0.706) {
        return (q * q / 2.0) - (7.0 * qPow(q, 4) / 128.0) + (29.0 * qPow(q, 6) / 2304.0) -
               (68687.0 * qPow(q, 8) / 18874368.0);
    } else if (q <= 0.908) {
        double q0 = 0.706;
        double a0 = (q0 * q0 / 2.0) - (7.0 * qPow(q0, 4) / 128.0) + (29.0 * qPow(q0, 6) / 2304.0) -
                    (68687.0 * qPow(q0, 8) / 18874368.0);
        double t = (q - q0) / (0.908 - q0);
        return a0 * (1.0 - t);
    }
    return 0.0;
}

void StabilityRegionPlotter::plotPoint(double q, double a) {
    if (!m_plot)
        return;
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
