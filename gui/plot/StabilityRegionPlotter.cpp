

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

double StabilityRegionPlotter::drawVerticalDistanceIndicator(double q, double a,
                                                             double a_boundary) {
    if (!m_plot)
        return 0.0;
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
    return a_boundary - a;
}

double StabilityRegionPlotter::drawLeftHorizontalDistanceIndicator(double q, double a) {
    if (!m_plot)
        return 0.0;
    double min_q = 0.0;
    double left_q = q;
    for (double test_q = q; test_q >= min_q; test_q -= 0.001) {
        double test_a = StabilityCalculator::calculateUpperBoundary(test_q);
        if (std::abs(test_a - a) < 1e-4 || test_a < a) {
            left_q = test_q;
            break;
        }
    }
    if (m_leftHorizontalLine) {
        m_plot->removeItem(m_leftHorizontalLine);
        m_leftHorizontalLine = nullptr;
    }
    m_leftHorizontalLine = new QCPItemLine(m_plot);
    m_leftHorizontalLine->setObjectName("leftHorizontalDistanceLine");
    m_leftHorizontalLine->start->setType(QCPItemPosition::ptPlotCoords);
    m_leftHorizontalLine->end->setType(QCPItemPosition::ptPlotCoords);
    m_leftHorizontalLine->start->setCoords(q, a);
    m_leftHorizontalLine->end->setCoords(left_q, a);
    QPen leftPen(Qt::darkCyan);
    leftPen.setWidth(2);
    m_leftHorizontalLine->setPen(leftPen);
    m_plot->replot();
    return left_q - q;
}

double StabilityRegionPlotter::drawRightHorizontalDistanceIndicator(double q, double a) {
    if (!m_plot)
        return 0.0;
    double max_q = 0.908;
    double right_q = q;
    for (double test_q = q; test_q <= max_q; test_q += 0.001) {
        double test_a = StabilityCalculator::calculateUpperBoundary(test_q);
        if (std::abs(test_a - a) < 1e-4 || test_a < a) {
            right_q = test_q;
            break;
        }
    }
    if (m_rightHorizontalLine) {
        m_plot->removeItem(m_rightHorizontalLine);
        m_rightHorizontalLine = nullptr;
    }
    m_rightHorizontalLine = new QCPItemLine(m_plot);
    m_rightHorizontalLine->setObjectName("rightHorizontalDistanceLine");
    m_rightHorizontalLine->start->setType(QCPItemPosition::ptPlotCoords);
    m_rightHorizontalLine->end->setType(QCPItemPosition::ptPlotCoords);
    m_rightHorizontalLine->start->setCoords(q, a);
    m_rightHorizontalLine->end->setCoords(right_q, a);
    QPen rightPen(Qt::darkYellow);
    rightPen.setWidth(2);
    m_rightHorizontalLine->setPen(rightPen);
    m_plot->replot();
    return right_q - q;
}

std::tuple<double, double, double> StabilityRegionPlotter::drawDistanceIndicators(
    double q, double a, double a_boundary) {
    double vertical = 0.0, left = 0.0, right = 0.0;
    if (a < a_boundary) {
        vertical = drawVerticalDistanceIndicator(q, a, a_boundary);
        left = drawLeftHorizontalDistanceIndicator(q, a);
        right = drawRightHorizontalDistanceIndicator(q, a);
    }
    return {vertical, left, right};
}
