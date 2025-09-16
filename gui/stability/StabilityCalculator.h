#ifndef STABILITYCALCULATOR_H
#define STABILITYCALCULATOR_H

#include <QtMath>

namespace StabilityCalculator {
double calculateUpperBoundary(double q);
double verticalDistance(double a, double a_boundary);
double horizontalDistance(double q, double q_boundary);
double euclideanDistance(double a, double a_boundary, double q, double q_boundary);
double angularOffset(double a, double a_boundary, double q, double q_boundary);
double normalizedStabilityMargin(double a, double a_boundary, double q, double q_boundary);
double worstCaseBoundaryProximity(const QVector<double>& a_boundaries, double a, double q,
                                  double q_boundary);

}  // namespace StabilityCalculator
#endif  // STABILITYCALCULATOR_H
