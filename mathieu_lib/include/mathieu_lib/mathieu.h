#pragma once

namespace mathieu_lib {

struct QuadrupoleParams {
    double frequency;    // Frequency in Hz
    double quad_radius;  // Characteristic dimension of the quadrupole in meters
    double molar_mass;   // Molar mass in kg/mol

    QuadrupoleParams(double freq, double radius,  // NOLINT(bugprone-easily-swappable-parameters)
                     double mass)
        : frequency(freq), quad_radius(radius), molar_mass(mass) {}
};

constexpr double AVOGADRO_NUMBER = 6.02214076E23;
constexpr double MIN_Q = 0.25;
constexpr double MAX_Q = 0.908;

auto omega(double frequency) -> double;
auto particle_mass(double molar_mass) -> double;
auto beta(double mathieu_q) -> double;
auto secular_frequency(double frequency, double mathieu_q) -> double;
auto mathieu_q(double voltage_rf, int charge_state, const QuadrupoleParams& params) -> double;
auto mathieu_a(double voltage_dc, int charge_state, const QuadrupoleParams& params) -> double;
auto mz(double voltage_rf, int charge_state, const QuadrupoleParams& params,
        double mathieu_q) -> double;
auto lmco(double voltage_rf, int charge_state, const QuadrupoleParams& params,
          double max_q) -> double;
auto max_mz(double voltage_rf, int charge_state, const QuadrupoleParams& params,
            double max_q) -> double;
}  // namespace mathieu_lib
