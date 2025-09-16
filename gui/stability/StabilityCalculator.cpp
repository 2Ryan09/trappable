
#include "StabilityCalculator.h"

#include <QVector>

namespace StabilityCalculator {
double calculateUpperBoundary(double q) {
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

double verticalDistance(double a, double a_boundary) { return a_boundary - a; }

double horizontalDistance(double q, double q_boundary) { return q_boundary - q; }

double euclideanDistance(double a, double a_boundary, double q, double q_boundary) {
    double da = verticalDistance(a, a_boundary);
    double dq = horizontalDistance(q, q_boundary);
    return qSqrt(da * da + dq * dq);
}

double angularOffset(double a, double a_boundary, double q, double q_boundary) {
    double da = verticalDistance(a, a_boundary);
    double dq = horizontalDistance(q, q_boundary);
    return qRadiansToDegrees(qAtan2(da, dq));
}

double normalizedStabilityMargin(double a, double a_boundary, double q, double q_boundary) {
    double de = euclideanDistance(a, a_boundary, q, q_boundary);
    double a_max = a_boundary;  // For now, use a_boundary as max
    return de / qSqrt(a_max * a_max + q_boundary * q_boundary);
}

double worstCaseBoundaryProximity(const QVector<double>& a_boundaries, double a, double q,
                                  double q_boundary) {
    double min_dist = 1e9;
    for (double a_b : a_boundaries) {
        double dist = euclideanDistance(a, a_b, q, q_boundary);
        if (dist < min_dist)
            min_dist = dist;
    }
    return min_dist;
}
}  // namespace StabilityCalculator