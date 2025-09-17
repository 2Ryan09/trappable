#include <gtest/gtest.h>

#include <QVector2D>

#include "stability/StabilityCalculator.h"

using namespace StabilityCalculator;

TEST(StabilityCalculatorTest, FindNearestBoundaryPoint) {
    auto [q_b, a_b] = findNearestBoundaryPoint(0.5, 0.1);
    // The nearest boundary point should be self-consistent
    auto [q_b2, a_b2] = findNearestBoundaryPoint(q_b, a_b);
    EXPECT_NEAR(q_b, q_b2, 1e-6);
    EXPECT_NEAR(a_b, a_b2, 1e-6);
}

TEST(StabilityCalculatorTest, VerticalDistance) {
    double a = 0.1, q = 0.5;
    double dist = verticalDistance(a, q);
    auto [q_b, a_b] = findNearestBoundaryPoint(q, a);
    double expected = a_b - a;
    EXPECT_NEAR(dist, expected, 1e-6);
}

TEST(StabilityCalculatorTest, HorizontalDistance) {
    double a = 0.1, q = 0.5;
    double dist = horizontalDistance(q, a);
    EXPECT_NEAR(dist, 0.0, 0.01);  // Should be near zero for q on boundary
}

TEST(StabilityCalculatorTest, EuclideanDistance) {
    double a = 0.1, q = 0.5;
    double dist = euclideanDistance(a, q);
    auto [q_b, a_b] = findNearestBoundaryPoint(q, a);
    double expected = qSqrt((q - q_b) * (q - q_b) + (a - a_b) * (a - a_b));
    EXPECT_NEAR(dist, expected, 1e-6);
}

TEST(StabilityCalculatorTest, AngularOffset) {
    double a = 0.1, q = 0.5;
    double q_b = 0.5, a_b = calculateUpperBoundary(0.5);
    double angle = angularOffset(a, a_b, q, q_b);
    EXPECT_GE(angle, 0.0);
    EXPECT_LE(angle, 90.0);
}

TEST(StabilityCalculatorTest, NormalizedStabilityMargin) {
    double a = 0.1, q = 0.5;
    double q_b = 0.5, a_b = calculateUpperBoundary(0.5);
    double margin = normalizedStabilityMargin(a, a_b, q, q_b);
    EXPECT_GE(margin, 0.0);
}
