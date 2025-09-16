#pragma once

#include <QString>

class MathieuBackend {
   public:
    MathieuBackend() = default;
    void calculate(double frequency, double radius, double mass, double voltageRf,
                   double voltageRfMax, double voltageDc, int chargeState);

    QString omega() const;
    QString particleMass() const;
    QString mathieuQ() const;
    QString mathieuA() const;
    QString beta() const;
    QString secularFrequency() const;
    QString mz() const;
    QString lmco() const;
    QString maxMz() const;

   private:
    QString m_omega, m_particleMass, m_mathieuQ, m_mathieuA, m_beta, m_secularFrequency, m_mz,
        m_lmco, m_maxMz;
};
