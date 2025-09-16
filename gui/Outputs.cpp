#include "Outputs.h"

#include <QString>
#include <cmath>

Outputs::Outputs(QWidget* parent) : QWidget(parent) {
    auto* group = new QGroupBox("Outputs", this);
    QFont titleFont = group->font();
    titleFont.setBold(true);
    group->setFont(titleFont);
    auto* layout = new QGridLayout(group);
    int row = 0;
    omegaValueLabel = new QLabel("", this);
    omegaUnitLabel = new QLabel("Hz", this);
    layout->addWidget(new QLabel("Omega:"), row, 0);
    layout->addWidget(omegaValueLabel, row, 1);
    layout->addWidget(omegaUnitLabel, row++, 2);

    particleMassValueLabel = new QLabel("", this);
    particleMassUnitLabel = new QLabel("kg", this);
    layout->addWidget(new QLabel("Particle mass:"), row, 0);
    layout->addWidget(particleMassValueLabel, row, 1);
    layout->addWidget(particleMassUnitLabel, row++, 2);

    mathieuQValueLabel = new QLabel("", this);
    mathieuQUnitLabel = new QLabel("", this);
    layout->addWidget(new QLabel("Mathieu q:"), row, 0);
    layout->addWidget(mathieuQValueLabel, row, 1);
    layout->addWidget(mathieuQUnitLabel, row++, 2);

    mathieuAValueLabel = new QLabel("", this);
    mathieuAUnitLabel = new QLabel("", this);
    layout->addWidget(new QLabel("Mathieu a:"), row, 0);
    layout->addWidget(mathieuAValueLabel, row, 1);
    layout->addWidget(mathieuAUnitLabel, row++, 2);

    betaValueLabel = new QLabel("", this);
    betaUnitLabel = new QLabel("", this);
    layout->addWidget(new QLabel("Beta:"), row, 0);
    layout->addWidget(betaValueLabel, row, 1);
    layout->addWidget(betaUnitLabel, row++, 2);

    secularFrequencyValueLabel = new QLabel("", this);
    secularFrequencyUnitLabel = new QLabel("Hz", this);
    layout->addWidget(new QLabel("Secular frequency:"), row, 0);
    layout->addWidget(secularFrequencyValueLabel, row, 1);
    layout->addWidget(secularFrequencyUnitLabel, row++, 2);

    mzValueLabel = new QLabel("", this);
    mzUnitLabel = new QLabel("Da", this);
    layout->addWidget(new QLabel("m/z:"), row, 0);
    layout->addWidget(mzValueLabel, row, 1);
    layout->addWidget(mzUnitLabel, row++, 2);

    lmcoValueLabel = new QLabel("", this);
    lmcoUnitLabel = new QLabel("Da", this);
    layout->addWidget(new QLabel("LMCO:"), row, 0);
    layout->addWidget(lmcoValueLabel, row, 1);
    layout->addWidget(lmcoUnitLabel, row++, 2);

    maxMzValueLabel = new QLabel("", this);
    maxMzUnitLabel = new QLabel("Da", this);
    layout->addWidget(new QLabel("Max m/z:"), row, 0);
    layout->addWidget(maxMzValueLabel, row, 1);
    layout->addWidget(maxMzUnitLabel, row++, 2);

    setLayout(layout);
}

void Outputs::setInvalid() {
    omegaValueLabel->setText("Invalid");
    particleMassValueLabel->setText("Invalid");
    mathieuQValueLabel->setText("Invalid");
    mathieuAValueLabel->setText("Invalid");
    betaValueLabel->setText("Invalid");
    secularFrequencyValueLabel->setText("Invalid");
    mzValueLabel->setText("Invalid");
    lmcoValueLabel->setText("Invalid");
    maxMzValueLabel->setText("Invalid");
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
