#include <QApplication>
#include <QDoubleValidator>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include "mathieu_lib/mathieu.h"

constexpr double MAX_Q = 0.908;

auto main(int argc, char *argv[]) -> int {
    QApplication app(argc, argv);

    auto *window = new QWidget;
    window->setWindowTitle("Mathieu Quadrupole Calculator");

    auto *mainLayout = new QVBoxLayout(window);
    auto *formLayout = new QFormLayout;

    // Input fields for QuadrupoleParams and all equation parameters
    auto *frequencyEdit = new QLineEdit;
    auto *radiusEdit = new QLineEdit;
    auto *massEdit = new QLineEdit;
    auto *voltageRfEdit = new QLineEdit;
    auto *voltageRfMaxEdit = new QLineEdit;
    auto *voltageDcEdit = new QLineEdit;
    auto *chargeStateEdit = new QLineEdit;

    auto freqValidator = std::make_unique<QDoubleValidator>();
    auto radiusValidator = std::make_unique<QDoubleValidator>();
    auto massValidator = std::make_unique<QDoubleValidator>();
    auto voltageRfValidator = std::make_unique<QDoubleValidator>();
    auto voltageRfMaxValidator = std::make_unique<QDoubleValidator>();
    auto voltageDcValidator = std::make_unique<QDoubleValidator>();
    auto chargeStateValidator = std::make_unique<QIntValidator>();

    frequencyEdit->setValidator(freqValidator.get());
    radiusEdit->setValidator(radiusValidator.get());
    massEdit->setValidator(massValidator.get());
    voltageRfEdit->setValidator(voltageRfValidator.get());
    voltageRfMaxEdit->setValidator(voltageRfMaxValidator.get());
    voltageDcEdit->setValidator(voltageDcValidator.get());
    chargeStateEdit->setValidator(chargeStateValidator.get());

    formLayout->addRow("Frequency (Hz):", frequencyEdit);
    formLayout->addRow("Quadrupole radius (m):", radiusEdit);
    formLayout->addRow("Molar mass (kg/mol):", massEdit);
    formLayout->addRow("RF Voltage (V):", voltageRfEdit);
    formLayout->addRow("RF Voltage (max, V):", voltageRfMaxEdit);
    formLayout->addRow("DC Voltage (V):", voltageDcEdit);
    formLayout->addRow("Charge state:", chargeStateEdit);

    mainLayout->addLayout(formLayout);

    auto *calcButton = new QPushButton("Calculate");
    mainLayout->addWidget(calcButton);

    auto *resultLabel = new QLabel;
    mainLayout->addWidget(resultLabel);

    QObject::connect(
        calcButton, &QPushButton::clicked,
        [frequencyEdit, radiusEdit, massEdit, voltageRfEdit, voltageRfMaxEdit, voltageDcEdit,
         chargeStateEdit, resultLabel]() {
            bool ok_freq = false;
            bool ok_radius = false;
            bool ok_mass = false;
            bool ok_voltage_rf = false;
            bool ok_voltage_rf_max = false;
            bool ok_voltage_dc = false;
            bool ok_charge_state = false;

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

            const QString res =
                QString(
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

    window->setLayout(mainLayout);
    window->show();
    return QApplication::exec();
}
