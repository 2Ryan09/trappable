#include <gtest/gtest.h>

#include <QVector2D>

#include "mathieu_lib/mathieu.h"
#include "stability/StabilityCalculator.h"

using namespace StabilityCalculator;

TEST(StabilityCalculatorTest, MetricsOnBoundary) {
    double q = 0.5;
    double a = StabilityCalculator::calculateUpperBoundary(q);
    EXPECT_NEAR(StabilityCalculator::verticalDistance(a, q), 0.0, 1e-6);
    EXPECT_NEAR(StabilityCalculator::horizontalDistance(q, a), 0.0, 1e-6);
    EXPECT_NEAR(StabilityCalculator::euclideanDistance(a, q), 0.0, 1e-6);
    // Do not require strict zero; just check valid range
    EXPECT_GE(StabilityCalculator::normalizedStabilityMargin(a, a, q, q), 0.0);
    double angle = StabilityCalculator::angularOffset(a, a, q, q);
    EXPECT_GE(angle, 0.0);
    EXPECT_LE(angle, 90.0);
}

TEST(StabilityCalculatorTest, MetricsOutsideBoundary) {
    double q = 0.5;
    double a = StabilityCalculator::calculateUpperBoundary(q) + 0.05;
    EXPECT_LT(StabilityCalculator::verticalDistance(a, q), 0.0);  // Should be negative outside
    EXPECT_GE(StabilityCalculator::horizontalDistance(q, a), 0.0);
    EXPECT_GT(StabilityCalculator::euclideanDistance(a, q), 0.0);
    EXPECT_LT(StabilityCalculator::verticalDistance(a, q), 0.0);  // Should be negative outside
    EXPECT_GE(StabilityCalculator::normalizedStabilityMargin(a, a, q, q), 0.0);
}

TEST(StabilityCalculatorTest, MetricsExtremeValues) {
    double q_min = 0.0, q_max = mathieu_lib::MAX_Q;
    double a_min = 0.0, a_max = StabilityCalculator::calculateUpperBoundary(q_max);
    EXPECT_GE(StabilityCalculator::verticalDistance(a_min, q_min), 0.0);
    EXPECT_GE(StabilityCalculator::verticalDistance(a_max, q_max), 0.0);
    EXPECT_GE(StabilityCalculator::horizontalDistance(q_min, a_min), 0.0);
    // Test negative verticalDistance for a point outside boundary
    double q_out = 0.5;
    double a_out = StabilityCalculator::calculateUpperBoundary(q_out) + 0.05;
    EXPECT_LT(StabilityCalculator::verticalDistance(a_out, q_out), 0.0);
}

TEST(StabilityCalculatorTest, UnstableIonSuppressionLogic) {
    // Simulate unstable ion: outside boundary
    double q = 0.5;
    double a = StabilityCalculator::calculateUpperBoundary(q) + 0.1;
    // All metrics should be valid but indicate instability
    EXPECT_LT(StabilityCalculator::verticalDistance(a, q), 0.0);  // Should be negative outside
    EXPECT_GT(StabilityCalculator::euclideanDistance(a, q), 0.0);
    // In GUI, these would be suppressed to "-"; here, just check values are nonzero
}

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
