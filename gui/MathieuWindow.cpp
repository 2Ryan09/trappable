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
    auto* mainLayout = new QVBoxLayout(this);
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
    mainLayout->addLayout(formLayout);

    calcButton = new QPushButton("Calculate");
    calcButton->setObjectName("calcButton");
    mainLayout->addWidget(calcButton);

    resultLabel = new QLabel;
    resultLabel->setObjectName("resultLabel");
    mainLayout->addWidget(resultLabel);

    // Add the stability plot widget
    stabilityPlot = new MathieuStabilityPlot(this);
    mainLayout->addWidget(stabilityPlot);

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
            resultLabel->setText("Please enter valid numbers for all fields.");
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
        const QString res = QString(
                                "omega: %1\nparticle_mass: %2\nmathieu_q: %3\nmathieu_a: %4\nbeta: "
                                "%5\nsecular_frequency: %6\nm/z: %7\nlmco: %8\nmax_mz: %9")
                                .arg(omega_val)
                                .arg(particle_mass_val)
                                .arg(mathieu_q_val)
                                .arg(mathieu_a_val)
                                .arg(beta_val)
                                .arg(secular_freq_val)
                                .arg(mz_val)
                                .arg(lmco_val)
                                .arg(max_mz_val);
        resultLabel->setText(res);
    });
}

MathieuWindow::~MathieuWindow() {
    // All child widgets are deleted by Qt's parent-child mechanism
}
