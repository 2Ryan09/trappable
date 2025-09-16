#include "MathieuWindow.h"

#include <QDoubleValidator>
#include <QFormLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "mathieu_lib/mathieu.h"

constexpr double MAX_Q = 0.908;

MathieuWindow::MathieuWindow(QWidget* parent) : QWidget(parent) {
    // Use a horizontal layout: left = inputs/results, right = plot
    auto* mainLayout = new QHBoxLayout(this);

    // Left side: inputs and outputs
    auto* leftWidget = new QWidget(this);
    auto* leftLayout = new QVBoxLayout(leftWidget);
    auto* formLayout = new QFormLayout;

    frequencyEdit = new QLineEdit;
    frequencyEdit->setObjectName("frequencyEdit");
    radiusEdit = new QLineEdit;
    radiusEdit->setObjectName("radiusEdit");
    massEdit = new QLineEdit;
    massEdit->setObjectName("massEdit");
    voltageRfEdit = new QLineEdit;
    voltageRfEdit->setObjectName("voltageRfEdit");
    voltageRfMaxEdit = new QLineEdit;
    voltageRfMaxEdit->setObjectName("voltageRfMaxEdit");
    voltageDcEdit = new QLineEdit;
    voltageDcEdit->setObjectName("voltageDcEdit");
    chargeStateEdit = new QLineEdit;
    chargeStateEdit->setObjectName("chargeStateEdit");

    frequencyEdit->setValidator(new QDoubleValidator(this));
    radiusEdit->setValidator(new QDoubleValidator(this));
    massEdit->setValidator(new QDoubleValidator(this));
    voltageRfEdit->setValidator(new QDoubleValidator(this));
    voltageRfMaxEdit->setValidator(new QDoubleValidator(this));
    voltageDcEdit->setValidator(new QDoubleValidator(this));
    chargeStateEdit->setValidator(new QIntValidator(this));

    formLayout->addRow("Frequency (Hz):", frequencyEdit);
    formLayout->addRow("Quadrupole radius (m):", radiusEdit);
    formLayout->addRow("Molar mass (kg/mol):", massEdit);
    formLayout->addRow("RF Voltage (V):", voltageRfEdit);
    formLayout->addRow("RF Voltage (max, V):", voltageRfMaxEdit);
    formLayout->addRow("DC Voltage (V):", voltageDcEdit);
    formLayout->addRow("Charge state:", chargeStateEdit);
    leftLayout->addLayout(formLayout);

    calcButton = new QPushButton("Calculate");
    calcButton->setObjectName("calcButton");
    leftLayout->addWidget(calcButton);

    QGridLayout* outputLayout = new QGridLayout;
    int row = 0;
    omegaValueLabel = new QLabel;
    omegaUnitLabel = new QLabel("Hz");
    outputLayout->addWidget(new QLabel("Omega:"), row, 0);
    outputLayout->addWidget(omegaValueLabel, row, 1);
    outputLayout->addWidget(omegaUnitLabel, row++, 2);

    particleMassValueLabel = new QLabel;
    particleMassUnitLabel = new QLabel("kg");
    outputLayout->addWidget(new QLabel("Particle mass:"), row, 0);
    outputLayout->addWidget(particleMassValueLabel, row, 1);
    outputLayout->addWidget(particleMassUnitLabel, row++, 2);

    mathieuQValueLabel = new QLabel;
    mathieuQUnitLabel = new QLabel("");
    outputLayout->addWidget(new QLabel("Mathieu q:"), row, 0);
    outputLayout->addWidget(mathieuQValueLabel, row, 1);
    outputLayout->addWidget(mathieuQUnitLabel, row++, 2);

    mathieuAValueLabel = new QLabel;
    mathieuAUnitLabel = new QLabel("");
    outputLayout->addWidget(new QLabel("Mathieu a:"), row, 0);
    outputLayout->addWidget(mathieuAValueLabel, row, 1);
    outputLayout->addWidget(mathieuAUnitLabel, row++, 2);

    betaValueLabel = new QLabel;
    betaUnitLabel = new QLabel("");
    outputLayout->addWidget(new QLabel("Beta:"), row, 0);
    outputLayout->addWidget(betaValueLabel, row, 1);
    outputLayout->addWidget(betaUnitLabel, row++, 2);

    secularFrequencyValueLabel = new QLabel;
    secularFrequencyUnitLabel = new QLabel("Hz");
    outputLayout->addWidget(new QLabel("Secular frequency:"), row, 0);
    outputLayout->addWidget(secularFrequencyValueLabel, row, 1);
    outputLayout->addWidget(secularFrequencyUnitLabel, row++, 2);

    mzValueLabel = new QLabel;
    mzUnitLabel = new QLabel("Da");
    outputLayout->addWidget(new QLabel("m/z:"), row, 0);
    outputLayout->addWidget(mzValueLabel, row, 1);
    outputLayout->addWidget(mzUnitLabel, row++, 2);

    lmcoValueLabel = new QLabel;
    lmcoUnitLabel = new QLabel("Da");
    outputLayout->addWidget(new QLabel("LMCO:"), row, 0);
    outputLayout->addWidget(lmcoValueLabel, row, 1);
    outputLayout->addWidget(lmcoUnitLabel, row++, 2);

    maxMzValueLabel = new QLabel;
    maxMzUnitLabel = new QLabel("Da");
    outputLayout->addWidget(new QLabel("Max m/z:"), row, 0);
    outputLayout->addWidget(maxMzValueLabel, row, 1);
    outputLayout->addWidget(maxMzUnitLabel, row++, 2);

    leftLayout->addLayout(outputLayout);

    leftLayout->addStretch();
    mainLayout->addWidget(leftWidget, 0);  // left side, stretch factor 0

    // Right side: large stability plot
    stabilityPlotWidget = new QCustomPlot(this);
    stabilityPlotWidget->setMinimumWidth(600);
    stabilityPlotWidget->setMinimumHeight(400);
    mainLayout->addWidget(stabilityPlotWidget, 1);  // right side, stretch factor 1

    stabilityPlotter = new StabilityRegionPlotter(stabilityPlotWidget);

    connect(calcButton, &QPushButton::clicked, this, [this]() {
        bool ok_freq = false, ok_radius = false, ok_mass = false, ok_voltage_rf = false,
             ok_voltage_rf_max = false, ok_voltage_dc = false, ok_charge_state = false;
        double freq = frequencyEdit->text().toDouble(&ok_freq);
        double radius = radiusEdit->text().toDouble(&ok_radius);
        double mass = massEdit->text().toDouble(&ok_mass);
        double voltage_rf = voltageRfEdit->text().toDouble(&ok_voltage_rf);
        double voltage_rf_max = voltageRfMaxEdit->text().toDouble(&ok_voltage_rf_max);
        double voltage_dc = voltageDcEdit->text().toDouble(&ok_voltage_dc);
        int charge_state = chargeStateEdit->text().toInt(&ok_charge_state);
        if (!(ok_freq && ok_radius && ok_mass && ok_voltage_rf && ok_voltage_rf_max &&
              ok_voltage_dc && ok_charge_state)) {
            omegaValueLabel->setText("Invalid");
            particleMassValueLabel->setText("Invalid");
            mathieuQValueLabel->setText("Invalid");
            mathieuAValueLabel->setText("Invalid");
            betaValueLabel->setText("Invalid");
            secularFrequencyValueLabel->setText("Invalid");
            mzValueLabel->setText("Invalid");
            lmcoValueLabel->setText("Invalid");
            maxMzValueLabel->setText("Invalid");
            return;
        }
        mathieu_lib::QuadrupoleParams params(freq, radius, mass);
        double omega_val = mathieu_lib::omega(freq);
        double particle_mass_val = mathieu_lib::particle_mass(mass);
        double mathieu_q_val = mathieu_lib::mathieu_q(voltage_rf, charge_state, params);
        double mathieu_a_val = mathieu_lib::mathieu_a(voltage_dc, charge_state, params);
        double beta_val = mathieu_lib::beta(mathieu_q_val);
        double secular_freq_val = mathieu_lib::secular_frequency(freq, mathieu_q_val);
        double mz_val = mathieu_lib::mz(voltage_rf, charge_state, params, mathieu_q_val);
        double lmco_val = mathieu_lib::lmco(voltage_rf, charge_state, params, MAX_Q);
        double max_mz_val = mathieu_lib::max_mz(voltage_rf_max, charge_state, params, MAX_Q);
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
        // Plot the calculated Mathieu q and a on the stability plot
        stabilityPlotter->plotPoint(mathieu_q_val, mathieu_a_val);
    });
}

MathieuWindow::~MathieuWindow() {
    // All child widgets are deleted by Qt's parent-child mechanism
}
