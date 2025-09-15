
// NOLINTBEGIN(readability-magic-numbers, bugprone-easily-swappable-parameters)

/**
 * @file mathieu.cpp
 * @brief Implementation of mathieu_lib functions.
 */
#include "mathieu_lib/mathieu.h"

#include <cmath>

constexpr double E_CHARGE = 1.602176E-19;          // elementary charge in coulombs
constexpr double AVOGADRO_NUMBER = 6.02214076E23;  // Avogadro's number in mol^-1
constexpr double MIN_Q = 0.25;                     // Minimum stable q value for quadrupole
constexpr double MAX_Q = 0.908;                    // Maximum stable q value for quadrupole

/**
 * @namespace mathieu_lib
 * @brief Namespace for Mathieu-related mathematical functions.
 */
namespace mathieu_lib {

/**
 * @brief Calculates the angular drive frequency (omega) for a given frequency.
 *
 * This function computes the angular drive frequency using the formula:
 * \f$ \omega = 2\pi f \f$
 *
 * @param frequency Frequency in Hz.
 * @return Angular drive frequency in radians per second.
 */
auto omega(double frequency) -> double { return 2.0 * M_PI * frequency; }

/**
 * @brief Converts molar mass to particle mass.
 *
 * This function converts the molar mass (in kg/mol) to the mass of a single particle (in kg)
 * using Avogadro's number.
 *
 * @param molar_mass Molar mass in kg/mol.
 * @return Particle mass in kg.
 */
auto particle_mass(double molar_mass) -> double { return molar_mass / AVOGADRO_NUMBER; }

/**
 * @brief Calculates the Mathieu q parameter for an ion in a quadrupole field.
 *
 * This function computes the Mathieu q parameter using the formula:
 * \f$ q = \frac{4 z e V_{rf}}{m r_0^2 \omega^2} \f$
 *
 * where:
 * - \f$ z \f$ is the charge state of the ion,
 * - \f$ e \f$ is the elementary charge,
 * - \f$ V_{rf} \f$ is the amplitude of the RF voltage,
 * - \f$ m \f$ is the mass of the ion,
 * - \f$ r_0 \f$ is the characteristic dimension of the quadrupole,
 * - \f$ \omega \f$ is the angular drive frequency.
 *
 * @param voltage_rf Amplitude of the RF voltage in volts.
 * @param charge_state Charge state of the ion (integer).
 * @param params Struct containing frequency, quad_radius, and molar_mass.
 * @return The Mathieu q parameter (dimensionless).
 */
auto mathieu_q(double voltage_rf, int charge_state, const QuadrupoleParams& params) -> double {
    const double omega_val = omega(params.frequency);
    const double mass = particle_mass(params.molar_mass);
    return (4.0 * charge_state * E_CHARGE * (voltage_rf / 2)) /
           (mass * omega_val * omega_val * params.quad_radius * params.quad_radius);
}

/**
 * @brief Calculates the Mathieu a parameter for an ion in a quadrupole field.
 *
 * This function computes the Mathieu a parameter using the formula:
 * \f$ a = \frac{8 z e V_{dc}}{m r_0^2 \omega^2} \f$
 *
 * where:
 * - \f$ z \f$ is the charge state of the ion,
 * - \f$ e \f$ is the elementary charge,
 * - \f$ V_{dc} \f$ is the amplitude of the DC voltage,
 * - \f$ m \f$ is the mass of the ion,
 * - \f$ r_0 \f$ is the characteristic dimension of the quadrupole,
 * - \f$ \omega \f$ is the angular drive frequency.
 *
 * @param voltage_dc Amplitude of the DC voltage in volts.
 * @param charge_state Charge state of the ion (integer).
 * @param params Struct containing frequency, quad_radius, and molar_mass.
 * @return The Mathieu a parameter (dimensionless).
 */
auto mathieu_a(double voltage_dc, int charge_state, const QuadrupoleParams& params) -> double {
    const double omega_val = omega(params.frequency);
    const double mass = particle_mass(params.molar_mass);
    return (8.0 * charge_state * E_CHARGE * voltage_dc) /
           (mass * omega_val * omega_val * params.quad_radius * params.quad_radius);
}

/**
 * @brief Calculates the m/z (mass-to-charge ratio) for a given Mathieu q parameter.
 *
 * This function computes the m/z using the formula:
 * \f$ m/z = \frac{4 V_{rf}}{(q/N_A) \omega^2 r_0^2} \f$
 *
 * where:
 * - \f$ V_{rf} \f$ is the amplitude of the RF voltage
 * - \f$ q \f$ is the Mathieu q parameter
 * - \f$ N_A \f$ is Avogadro's number
 * - \f$ \omega \f$ is the angular drive frequency
 * - \f$ r_0 \f$ is the characteristic dimension of the quadrupole
 *
 * @param voltage_rf Amplitude of the RF voltage in volts.
 * @param charge_state Charge state of the ion (integer).
 * @param frequency Frequency in Hz.
 * @param quad_radius Characteristic dimension of the quadrupole in meters.
 * @param molar_mass Molar mass in kg/mol.
 * @param mathieu_q The Mathieu q parameter (dimensionless).
 * @return The m/z (mass-to-charge ratio) in kg/mol.
 */
auto mz(double voltage_rf, int charge_state, const QuadrupoleParams& params,
        double mathieu_q) -> double {
    const double omega_val = omega(params.frequency);
    return (4.0 * (voltage_rf / 2) * E_CHARGE) /
           ((mathieu_q / AVOGADRO_NUMBER) * (omega_val * omega_val) *
            (params.quad_radius * params.quad_radius)) *
           1000;
}

/**
 * @brief Calculates the LMCO (Low Mass Cut Off) for a quadrupole mass filter.
 *
 * This function computes the LMCO using the formula:
 * \f$ LMCO = \frac{4 (V_{rf}/2)}{(q_{min}/N_A) \omega^2 r_0^2} \f$
 *
 * where:
 * - \f$ V_{rf} \f$ is the amplitude of the RF voltage
 * - \f$ q_{min} \f$ is the minimum stable q value (default is 0.25)
 * - \f$ N_A \f$ is Avogadro's number
 * - \f$ \omega \f$ is the angular drive frequency
 * - \f$ r_0 \f$ is the characteristic dimension of the quadrupole
 *
 * @param voltage_rf Amplitude of the RF voltage in volts.
 * @param charge_state Charge state of the ion (integer).
 * @param omega_val Angular drive frequency in radians per second.
 * @param quad_radius Characteristic dimension of the quadrupole in meters.
 * @param max_q Minimum stable q value (default is 0.908).
 * @return The LMCO in kg/mol.
 */
auto lmco(double voltage_rf, int charge_state, const QuadrupoleParams& params,
          double max_q = MAX_Q) -> double {
    return mz(voltage_rf, charge_state, params, max_q);
}

/**
 * @brief Calculates the maximum m/z.
 *
 * This function computes the maximum m/z using the formula:
 * \f$ \frac{4 V_{rf_max}}{(q_{max}/N_A) \omega^2 r_0^2} \f$
 *
 * where:
 * - \f$ V_{rf_max} \f$ is the maximum amplitude of the RF voltage
 * - \f$ q_{max} \f$ is the maximum stable q value (default is 0.908)
 * - \f$ N_A \f$ is Avogadro's number
 * - \f$ \omega \f$ is the angular drive frequency
 * - \f$ r_0 \f$ is the characteristic dimension of the quadrupole
 *
 * @param voltage_rf_max Maximum amplitude of the RF voltage in volts.
 * @param charge_state Charge state of the ion (integer).
 * @param frequency Frequency in Hz.
 * @param quad_radius Characteristic dimension of the quadrupole in meters.
 * @param molar_mass Molar mass in kg/mol.
 * @param max_q The maximum stable q value (dimensionless).
 * @return The maximum m/z in kg/mol.
 */
auto max_mz(double voltage_rf_max, int charge_state, const QuadrupoleParams& params,
            double max_q = MAX_Q) -> double {
    return mz(voltage_rf_max, charge_state, params, max_q);
}

/**
 * @brief Calculates the stability parameter beta for a given Mathieu q parameter.
 *
 * This function computes the stability parameter beta using the formula:
 * \f$ \beta = \frac{\sqrt{2}}{2q} \f$
 *
 * where:
 * - \f$ q \f$ is the Mathieu q parameter
 *
 * @param mathieu_q The Mathieu q parameter (dimensionless).
 * @return The stability parameter beta (dimensionless).
 */
auto beta(double mathieu_q) -> double { return (std::sqrt(2) / 2.0) * mathieu_q; }

/**
 * @brief Calculates the secular frequency for a given drive frequency and Mathieu q parameter.
 *
 * This function computes the secular frequency using the formula:
 * \f$ f_{secular} = \frac{\omega}{2\pi \beta} \f$
 *
 * where:
 * - \f$ \omega \f$ is the angular drive frequency
 * - \f$ \beta \f$ is the stability parameter
 *
 * The result is converted to kHz.
 *
 * @param frequency Frequency in Hz.
 * @param mathieu_q The Mathieu q parameter (dimensionless).
 * @return The secular frequency in kHz.
 */
auto secular_frequency(double frequency, double mathieu_q) -> double {
    return (omega(frequency) / (2 * M_PI) * (beta(mathieu_q) / 2)) / 1000;  // in kHz
}
}  // namespace mathieu_lib

// NOLINTEND(readability-magic-numbers, bugprone-easily-swappable-parameters)
