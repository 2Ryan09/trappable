
// NOLINTBEGIN(readability-magic-numbers)

#include <gtest/gtest.h>

#include <cmath>

#include "mathieu_lib/mathieu.h"
using namespace mathieu_lib;

// Helper for floating point comparison
constexpr double EPSILON = 1e-9;

TEST(MathieuTest, OmegaCalculation) {
    EXPECT_NEAR(mathieu_lib::omega(1.0), 2.0 * M_PI * 1.0, EPSILON);
    EXPECT_NEAR(mathieu_lib::omega(0.0), 0.0, EPSILON);
}

TEST(MathieuTest, ParticleMassCalculation) {
    EXPECT_NEAR(mathieu_lib::particle_mass(1.0), 1.0 / mathieu_lib::AVOGADRO_NUMBER, EPSILON);
    EXPECT_NEAR(mathieu_lib::particle_mass(0.0), 0.0, EPSILON);
}

TEST(MathieuTest, MathieuQBehavior) {
    mathieu_lib::QuadrupoleParams params(1e6, 0.01, 1.0);
    double q = mathieu_lib::mathieu_q(1000.0, 1, params);
    EXPECT_GT(q, 0.0);
    EXPECT_LT(q, 1.0);
}

TEST(MathieuTest, MathieuABehavior) {
    mathieu_lib::QuadrupoleParams params(1e6, 0.01, 1.0);
    double a = mathieu_lib::mathieu_a(500.0, 1, params);
    EXPECT_GT(a, 0.0);
}

TEST(MathieuTest, MZCalculation) {
    mathieu_lib::QuadrupoleParams params(1e6, 0.01, 1.0);
    double q = 0.5;
    double mz_val = mathieu_lib::mz(1000.0, 1, params, q);
    EXPECT_GT(mz_val, 0.0);
}

TEST(MathieuTest, LMCOCalculation) {
    mathieu_lib::QuadrupoleParams params(1e6, 0.01, 1.0);
    double lmco_val = mathieu_lib::lmco(1000.0, 1, params, mathieu_lib::MIN_Q);
    EXPECT_GT(lmco_val, 0.0);
}

TEST(MathieuTest, MaxMZCalculation) {
    mathieu_lib::QuadrupoleParams params(1e6, 0.01, 1.0);
    double max_mz_val = mathieu_lib::max_mz(1000.0, 1, params, mathieu_lib::MAX_Q);
    EXPECT_GT(max_mz_val, 0.0);
}

TEST(MathieuTest, BetaBehavior) {
    double q = 0.5;
    double beta_val = mathieu_lib::beta(q);
    EXPECT_GT(beta_val, 0.0);
}

TEST(MathieuTest, SecularFrequencyBehavior) {
    double freq = 1e6;
    double q = 0.5;
    double sec_freq = mathieu_lib::secular_frequency(freq, q);
    EXPECT_GT(sec_freq, 0.0);
}

// Edge cases
TEST(MathieuTest, ZeroChargeState) {
    mathieu_lib::QuadrupoleParams params(1e6, 0.01, 1.0);
    EXPECT_EQ(mathieu_lib::mathieu_q(1000.0, 0, params), 0.0);
    EXPECT_EQ(mathieu_lib::mathieu_a(500.0, 0, params), 0.0);
}

TEST(MathieuTest, NegativeInputs) {
    mathieu_lib::QuadrupoleParams params(-1e6, -0.01, -1.0);
    EXPECT_LT(mathieu_lib::mathieu_q(-1000.0, -1, params), 0.0);
    EXPECT_LT(mathieu_lib::mathieu_a(-500.0, -1, params), 0.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// NOLINTEND(readability-magic-numbers)
