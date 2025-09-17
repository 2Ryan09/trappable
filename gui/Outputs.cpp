#include "Outputs.h"

#include <QString>
#include <cmath>

Outputs::Outputs(QWidget* parent) : QWidget(parent) {
    auto* layout = new QGridLayout(this);

    int row = 0;
    omegaValueLabel = new QLabel("-", this);
    omegaValueLabel->setAlignment(Qt::AlignRight);
    omegaUnitLabel = new QLabel("Hz", this);
    auto* omegaLabel = new QLabel("Omega:");
    layout->addWidget(omegaLabel, row, 0);
    layout->addWidget(omegaValueLabel, row, 1);
    layout->addWidget(omegaUnitLabel, row++, 2);

    particleMassValueLabel = new QLabel("-", this);
    particleMassValueLabel->setAlignment(Qt::AlignRight);
    particleMassUnitLabel = new QLabel("kg", this);
    auto* massLabel = new QLabel("Particle mass:");
    layout->addWidget(massLabel, row, 0);
    layout->addWidget(particleMassValueLabel, row, 1);
    layout->addWidget(particleMassUnitLabel, row++, 2);

    mathieuQValueLabel = new QLabel("-", this);
    mathieuQValueLabel->setAlignment(Qt::AlignRight);
    mathieuQUnitLabel = new QLabel("", this);
    auto* qLabel = new QLabel("Mathieu q:");
    layout->addWidget(qLabel, row, 0);
    layout->addWidget(mathieuQValueLabel, row, 1);
    layout->addWidget(mathieuQUnitLabel, row++, 2);

    mathieuAValueLabel = new QLabel("-", this);
    mathieuAValueLabel->setAlignment(Qt::AlignRight);
    mathieuAUnitLabel = new QLabel("", this);
    auto* aLabel = new QLabel("Mathieu a:");
    layout->addWidget(aLabel, row, 0);
    layout->addWidget(mathieuAValueLabel, row, 1);
    layout->addWidget(mathieuAUnitLabel, row++, 2);

    betaValueLabel = new QLabel("-", this);
    betaValueLabel->setAlignment(Qt::AlignRight);
    betaUnitLabel = new QLabel("", this);
    auto* betaLabel = new QLabel("Beta:");
    layout->addWidget(betaLabel, row, 0);
    layout->addWidget(betaValueLabel, row, 1);
    layout->addWidget(betaUnitLabel, row++, 2);

    secularFrequencyValueLabel = new QLabel("-", this);
    secularFrequencyValueLabel->setAlignment(Qt::AlignRight);
    secularFrequencyUnitLabel = new QLabel("Hz", this);
    auto* secFreqLabel = new QLabel("Secular frequency:");
    layout->addWidget(secFreqLabel, row, 0);
    layout->addWidget(secularFrequencyValueLabel, row, 1);
    layout->addWidget(secularFrequencyUnitLabel, row++, 2);

    mzValueLabel = new QLabel("-", this);
    mzValueLabel->setAlignment(Qt::AlignRight);
    mzUnitLabel = new QLabel("Da", this);
    auto* mzLabel = new QLabel("m/z:");
    layout->addWidget(mzLabel, row, 0);
    layout->addWidget(mzValueLabel, row, 1);
    layout->addWidget(mzUnitLabel, row++, 2);

    lmcoValueLabel = new QLabel("-", this);
    lmcoValueLabel->setAlignment(Qt::AlignRight);
    lmcoUnitLabel = new QLabel("Da", this);
    auto* lmcoLabel = new QLabel("LMCO:");
    layout->addWidget(lmcoLabel, row, 0);
    layout->addWidget(lmcoValueLabel, row, 1);
    layout->addWidget(lmcoUnitLabel, row++, 2);

    maxMzValueLabel = new QLabel("-", this);
    maxMzValueLabel->setAlignment(Qt::AlignRight);
    maxMzUnitLabel = new QLabel("Da", this);
    auto* maxMzLabel = new QLabel("Max m/z:");
    layout->addWidget(maxMzLabel, row, 0);
    layout->addWidget(maxMzValueLabel, row, 1);
    layout->addWidget(maxMzUnitLabel, row++, 2);

    setLayout(layout);
}

void Outputs::setInvalid() {
    omegaValueLabel->setText("-");
    particleMassValueLabel->setText("-");
    mathieuQValueLabel->setText("-");
    mathieuAValueLabel->setText("-");
    betaValueLabel->setText("-");
    secularFrequencyValueLabel->setText("-");
    mzValueLabel->setText("-");
    lmcoValueLabel->setText("-");
    maxMzValueLabel->setText("-");
}

void Outputs::setValues(double omega_val, double particle_mass_val, double mathieu_q_val,
                        double mathieu_a_val, double beta_val, double secular_freq_val,
                        double mz_val, double lmco_val, double max_mz_val) {
    auto formatValue = [](double val) {
        double absVal = std::abs(val);
        if ((absVal > 0 && (absVal < 0.001 || absVal >= 10000))) {
            return QString::number(val, 'e', 3);  // scientific notation, 3 decimals
        } else {
            return QString::number(val, 'f', 3);  // fixed, 3 decimals
        }
    };
    omegaValueLabel->setText(formatValue(omega_val));
    particleMassValueLabel->setText(formatValue(particle_mass_val));
    mathieuQValueLabel->setText(formatValue(mathieu_q_val));
    mathieuAValueLabel->setText(formatValue(mathieu_a_val));
    betaValueLabel->setText(formatValue(beta_val));
    secularFrequencyValueLabel->setText(formatValue(secular_freq_val));
    mzValueLabel->setText(formatValue(mz_val));
    lmcoValueLabel->setText(formatValue(lmco_val));
    maxMzValueLabel->setText(formatValue(max_mz_val));
}
