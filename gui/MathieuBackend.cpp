#include "MathieuBackend.h"

#include <QString>

#include "mathieu_lib/mathieu.h"

void MathieuBackend::calculate(double frequency, double radius, double mass, double voltageRf,
                               double voltageRfMax, double voltageDc, int chargeState) {
    mathieu_lib::QuadrupoleParams params(frequency, radius, mass);
    constexpr double LMCO_MAGIC = 0.908;
    m_omega = QString::number(mathieu_lib::omega(frequency));
    m_particleMass = QString::number(mathieu_lib::particle_mass(mass));
    m_mathieuQ = QString::number(mathieu_lib::mathieu_q(voltageRf, chargeState, params));
    m_mathieuA = QString::number(mathieu_lib::mathieu_a(voltageDc, chargeState, params));
    m_beta =
        QString::number(mathieu_lib::beta(mathieu_lib::mathieu_q(voltageRf, chargeState, params)));
    m_secularFrequency = QString::number(mathieu_lib::secular_frequency(
        frequency, mathieu_lib::mathieu_q(voltageRf, chargeState, params)));
    m_mz = QString::number(mathieu_lib::mz(voltageRf, chargeState, params,
                                           mathieu_lib::mathieu_q(voltageRf, chargeState, params)));
    m_lmco = QString::number(mathieu_lib::lmco(voltageRf, chargeState, params, LMCO_MAGIC));
    m_maxMz = QString::number(mathieu_lib::max_mz(voltageRfMax, chargeState, params, LMCO_MAGIC));
}

QString MathieuBackend::omega() const { return m_omega; }
QString MathieuBackend::particleMass() const { return m_particleMass; }
QString MathieuBackend::mathieuQ() const { return m_mathieuQ; }
QString MathieuBackend::mathieuA() const { return m_mathieuA; }
QString MathieuBackend::beta() const { return m_beta; }
QString MathieuBackend::secularFrequency() const { return m_secularFrequency; }
QString MathieuBackend::mz() const { return m_mz; }
QString MathieuBackend::lmco() const { return m_lmco; }
QString MathieuBackend::maxMz() const { return m_maxMz; }