#include "StabilityCalculator.h"

#include <QVector2D>
#include <QVector>

#include "mathieu_lib/mathieu.h"

namespace StabilityCalculator {

std::pair<double, double> findNearestBoundaryPoint(double q, double a) {
    constexpr double min_q = 0.0;
    constexpr double max_q = mathieu_lib::MAX_Q;
    constexpr double coarse_step = 0.001;
    constexpr double fine_window = 0.01;
    constexpr double fine_step = 0.00001;
    double min_dist = 1e9;
    double best_q = q, best_a = a;
    // Coarse search
    for (double q_b = min_q; q_b <= max_q; q_b += coarse_step) {
        double a_b = calculateUpperBoundary(q_b);
        double dist = qSqrt((q - q_b) * (q - q_b) + (a - a_b) * (a - a_b));
        if (dist < min_dist) {
            min_dist = dist;
            best_q = q_b;
            best_a = a_b;
        }
    }
    // Fine search around best_q
    min_dist = 1e9;
    double fine_min_q = std::max(min_q, best_q - fine_window);
    double fine_max_q = std::min(max_q, best_q + fine_window);
    for (double q_b = fine_min_q; q_b <= fine_max_q; q_b += fine_step) {
        double a_b = calculateUpperBoundary(q_b);
        double dist = qSqrt((q - q_b) * (q - q_b) + (a - a_b) * (a - a_b));
        if (dist < min_dist) {
            min_dist = dist;
            best_q = q_b;
            best_a = a_b;
        }
    }
    return {best_q, best_a};
}

QVector2D boundaryTangent(double q_b) {
    constexpr double h = 1e-5;
    double a_b_plus = calculateUpperBoundary(q_b + h);
    double a_b_minus = calculateUpperBoundary(q_b - h);
    double da_dq = (a_b_plus - a_b_minus) / (2.0 * h);
    return QVector2D(1.0, da_dq).normalized();
}

double angleBetweenVectors(const QVector2D& v1, const QVector2D& v2) {
    double dot = QVector2D::dotProduct(v1, v2);
    double mag1 = v1.length();
    double mag2 = v2.length();
    double cos_theta = dot / (mag1 * mag2);
    cos_theta = std::max(-1.0, std::min(1.0, cos_theta));
    return qRadiansToDegrees(qAcos(cos_theta));
}
double calculateUpperBoundary(double q) {
    if (q <= 0.4) {
        return (q * q / 2.0) - (7.0 * qPow(q, 4) / 128.0);
    } else if (q <= 0.706) {
        return (q * q / 2.0) - (7.0 * qPow(q, 4) / 128.0) + (29.0 * qPow(q, 6) / 2304.0) -
               (68687.0 * qPow(q, 8) / 18874368.0);
    } else if (q <= mathieu_lib::MAX_Q) {
        double q0 = 0.706;
        double a0 = (q0 * q0 / 2.0) - (7.0 * qPow(q0, 4) / 128.0) + (29.0 * qPow(q0, 6) / 2304.0) -
                    (68687.0 * qPow(q0, 8) / 18874368.0);
        double t = (q - q0) / (mathieu_lib::MAX_Q - q0);
        return a0 * (1.0 - t);
    }
    return 0.0;
}

double verticalDistance(double a, double q) {
    auto [q_b, a_b] = findNearestBoundaryPoint(q, a);
    return a_b - a;
}

double horizontalDistance(double q, double a) {
    auto [q_b, a_b] = findNearestBoundaryPoint(q, a);
    return std::abs(q_b - q);
}

double euclideanDistance(double a, double q) {
    auto [q_b, a_b] = findNearestBoundaryPoint(q, a);
    return qSqrt((q - q_b) * (q - q_b) + (a - a_b) * (a - a_b));
}

double angularOffset(double a, double a_boundary, double q, double q_boundary) {
    // Find nearest boundary point
    auto [q_b, a_b] = findNearestBoundaryPoint(q, a);
    QVector2D op_vec(q_b - q, a_b - a);  // Vector from operating point to boundary
    QVector2D q_axis(1.0, 0.0);          // Reference: horizontal axis
    // Calculate angle using dot product, always in [0, 180]
    double dot = QVector2D::dotProduct(op_vec, q_axis);
    double mag_op = op_vec.length();
    double mag_q = q_axis.length();
    double cos_theta = dot / (mag_op * mag_q);
    cos_theta = std::max(-1.0, std::min(1.0, cos_theta));
    double angle_deg = qRadiansToDegrees(qAcos(cos_theta));
    // Clamp to [0, 90] for right triangle
    if (angle_deg > 90.0)
        angle_deg = 180.0 - angle_deg;
    return angle_deg;
}

double normalizedStabilityMargin(double a, double a_boundary, double q, double q_boundary) {
    double de = euclideanDistance(a, q);
    double a_max = a_boundary;  // For now, use a_boundary as max
    return de / qSqrt(a_max * a_max + q_boundary * q_boundary);
}
}  // namespace StabilityCalculator
