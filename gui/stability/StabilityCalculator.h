#include <QVector2D>

#ifndef STABILITYCALCULATOR_H
#define STABILITYCALCULATOR_H

#include <QtMath>

namespace StabilityCalculator {
double calculateUpperBoundary(double q);
double verticalDistance(double a, double a_boundary);
double horizontalDistance(double q, double a);
double euclideanDistance(double a, double q);
double angularOffset(double a, double a_boundary, double q, double q_boundary);
double normalizedStabilityMargin(double a, double a_boundary, double q, double q_boundary);
std::pair<double, double> findNearestBoundaryPoint(double q, double a);

}  // namespace StabilityCalculator
#endif  // STABILITYCALCULATOR_H
