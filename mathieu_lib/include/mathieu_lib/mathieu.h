#pragma once

#include <vector>

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
auto omega(const std::vector<double>& frequencies) -> std::vector<double>;

auto particle_mass(double molar_mass) -> double;
auto particle_mass(const std::vector<double>& molar_masses) -> std::vector<double>;

auto beta(double mathieu_q) -> double;
auto beta(const std::vector<double>& mathieu_qs) -> std::vector<double>;

auto secular_frequency(double frequency, double mathieu_q) -> double;
auto secular_frequency(const std::vector<double>& frequencies,
                       const std::vector<double>& mathieu_qs) -> std::vector<double>;

auto mathieu_q(double voltage_rf, int charge_state, const QuadrupoleParams& params) -> double;
auto mathieu_q(const std::vector<double>& voltage_rfs, const std::vector<int>& charge_states,
               const std::vector<QuadrupoleParams>& params) -> std::vector<double>;

auto mathieu_a(double voltage_dc, int charge_state, const QuadrupoleParams& params) -> double;
auto mathieu_a(const std::vector<double>& voltage_dcs, const std::vector<int>& charge_states,
               const std::vector<QuadrupoleParams>& params) -> std::vector<double>;

auto mz(double voltage_rf, int charge_state, const QuadrupoleParams& params,
        double mathieu_q) -> double;
auto mz(const std::vector<double>& voltage_rfs, const std::vector<int>& charge_states,
        const std::vector<QuadrupoleParams>& params,
        const std::vector<double>& mathieu_qs) -> std::vector<double>;

auto lmco(double voltage_rf, int charge_state, const QuadrupoleParams& params,
          double max_q) -> double;
auto lmco(const std::vector<double>& voltage_rfs, const std::vector<int>& charge_states,
          const std::vector<QuadrupoleParams>& params,
          const std::vector<double>& max_qs) -> std::vector<double>;

auto max_mz(double voltage_rf, int charge_state, const QuadrupoleParams& params,
            double max_q) -> double;
auto max_mz(const std::vector<double>& voltage_rfs, const std::vector<int>& charge_states,
            const std::vector<QuadrupoleParams>& params,
            const std::vector<double>& max_qs) -> std::vector<double>;
}  // namespace mathieu_lib
