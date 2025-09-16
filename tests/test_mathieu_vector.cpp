#include <gtest/gtest.h>

#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "mathieu_lib/mathieu.h"
using namespace mathieu_lib;

constexpr double EPSILON = 1e-9;

TEST(MathieuVectorTest, OmegaVector) {
    std::vector<double> freqs{1.0, 2.0, 0.0};
    auto result = omega(freqs);
    EXPECT_NEAR(result[0], 2.0 * M_PI * 1.0, EPSILON);
    EXPECT_NEAR(result[1], 2.0 * M_PI * 2.0, EPSILON);
    EXPECT_NEAR(result[2], 0.0, EPSILON);
}

TEST(MathieuVectorTest, ParticleMassVector) {
    std::vector<double> masses{1.0, 2.0, 0.0};
    auto result = particle_mass(masses);
    EXPECT_NEAR(result[0], 1.0 / AVOGADRO_NUMBER, EPSILON);
    EXPECT_NEAR(result[1], 2.0 / AVOGADRO_NUMBER, EPSILON);
    EXPECT_NEAR(result[2], 0.0, EPSILON);
}

TEST(MathieuVectorTest, MathieuQVector) {
    std::vector<double> v_rf{1000.0, 2000.0};
    std::vector<int> charge{1, 2};
    std::vector<QuadrupoleParams> params{QuadrupoleParams(1e6, 0.01, 1.0),
                                         QuadrupoleParams(2e6, 0.02, 2.0)};
    auto result = mathieu_q(v_rf, charge, params);
    EXPECT_EQ(result.size(), 2);
    EXPECT_GT(result[0], 0.0);
    EXPECT_GT(result[1], 0.0);
}

TEST(MathieuVectorTest, MathieuAVector) {
    std::vector<double> v_dc{500.0, 1000.0};
    std::vector<int> charge{1, 2};
    std::vector<QuadrupoleParams> params{QuadrupoleParams(1e6, 0.01, 1.0),
                                         QuadrupoleParams(2e6, 0.02, 2.0)};
    auto result = mathieu_a(v_dc, charge, params);
    EXPECT_EQ(result.size(), 2);
    EXPECT_GT(result[0], 0.0);
    EXPECT_GT(result[1], 0.0);
}

TEST(MathieuVectorTest, MZVector) {
    std::vector<double> v_rf{1000.0, 2000.0};
    std::vector<int> charge{1, 2};
    std::vector<QuadrupoleParams> params{QuadrupoleParams(1e6, 0.01, 1.0),
                                         QuadrupoleParams(2e6, 0.02, 2.0)};
    std::vector<double> q{0.5, 0.6};
    auto result = mz(v_rf, charge, params, q);
    EXPECT_EQ(result.size(), 2);
    EXPECT_GT(result[0], 0.0);
    EXPECT_GT(result[1], 0.0);
}

TEST(MathieuVectorTest, LMCOVector) {
    std::vector<double> v_rf{1000.0, 2000.0};
    std::vector<int> charge{1, 2};
    std::vector<QuadrupoleParams> params{QuadrupoleParams(1e6, 0.01, 1.0),
                                         QuadrupoleParams(2e6, 0.02, 2.0)};
    std::vector<double> max_q{MAX_Q, MAX_Q};
    auto result = lmco(v_rf, charge, params, max_q);
    EXPECT_EQ(result.size(), 2);
    EXPECT_GT(result[0], 0.0);
    EXPECT_GT(result[1], 0.0);
}

TEST(MathieuVectorTest, MaxMZVector) {
    std::vector<double> v_rf{1000.0, 2000.0};
    std::vector<int> charge{1, 2};
    std::vector<QuadrupoleParams> params{QuadrupoleParams(1e6, 0.01, 1.0),
                                         QuadrupoleParams(2e6, 0.02, 2.0)};
    std::vector<double> max_q{MAX_Q, MAX_Q};
    auto result = max_mz(v_rf, charge, params, max_q);
    EXPECT_EQ(result.size(), 2);
    EXPECT_GT(result[0], 0.0);
    EXPECT_GT(result[1], 0.0);
}

TEST(MathieuVectorTest, BetaVector) {
    std::vector<double> q{0.5, 0.6};
    auto result = beta(q);
    EXPECT_EQ(result.size(), 2);
    EXPECT_GT(result[0], 0.0);
    EXPECT_GT(result[1], 0.0);
}

TEST(MathieuVectorTest, SecularFrequencyVector) {
    std::vector<double> freqs{1e6, 2e6};
    std::vector<double> q{0.5, 0.6};
    auto result = secular_frequency(freqs, q);
    EXPECT_EQ(result.size(), 2);
    EXPECT_GT(result[0], 0.0);
    EXPECT_GT(result[1], 0.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
