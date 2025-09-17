#include "Inputs.h"

#include <QDoubleValidator>
#include <QIntValidator>

Inputs::Inputs(QWidget* parent) : QWidget(parent) {
    auto* layout = new QGridLayout(this);

    auto makeLineEdit = [](const char* name) {
        auto* edit = new QLineEdit;
        edit->setObjectName(QString::fromUtf8(name));
        return edit;
    };
    auto makeUnitRadios = [](const QString& left, const QString& right) {
        auto* leftRadio = new QRadioButton(left);
        auto* rightRadio = new QRadioButton(right);
        leftRadio->setChecked(true);
        auto* widget = new QWidget;
        auto* layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(leftRadio);
        layout->addWidget(rightRadio);
        widget->setLayout(layout);
        return std::make_tuple(leftRadio, rightRadio, widget);
    };

    auto makeUnitCombo = [](const QStringList& units) {
        auto* combo = new QComboBox;
        combo->addItems(units);
        combo->setCurrentIndex(0);  // Always select first by default
        return combo;
    };
    int inputRow = 0;

    // Frequency
    frequencyEdit = makeLineEdit("frequencyEdit");
    frequencyEdit->setValidator(new QDoubleValidator(0, 1e12, 6, frequencyEdit));
    frequencyEdit->setText("970000");
    frequencyUnitCombo = makeUnitCombo({"Hz", "kHz"});
    auto* freqLabel = new QLabel("Frequency:");
    layout->addWidget(freqLabel, inputRow, 0);
    layout->addWidget(frequencyEdit, inputRow, 1);
    layout->addWidget(frequencyUnitCombo, inputRow++, 2);

    // Radius
    radiusEdit = makeLineEdit("radiusEdit");
    radiusEdit->setValidator(new QDoubleValidator(0, 1e6, 6, radiusEdit));
    radiusEdit->setText("0.003478");
    radiusUnitCombo = makeUnitCombo({"m", "mm"});
    auto* radiusLabel = new QLabel("Quadrupole radius:");
    layout->addWidget(radiusLabel, inputRow, 0);
    layout->addWidget(radiusEdit, inputRow, 1);
    layout->addWidget(radiusUnitCombo, inputRow++, 2);

    // Mass
    massEdit = makeLineEdit("massEdit");
    massEdit->setValidator(new QDoubleValidator(0, 1e6, 6, massEdit));
    massEdit->setText("0.303");
    auto* massLabel = new QLabel("Molar mass:");
    auto* massUnitLabel = new QLabel("kg/mol");
    layout->addWidget(massLabel, inputRow, 0);
    layout->addWidget(massEdit, inputRow, 1);
    layout->addWidget(massUnitLabel, inputRow++, 2);

    // RF Voltage
    voltageRfEdit = makeLineEdit("voltageRfEdit");
    voltageRfEdit->setValidator(new QDoubleValidator(-1e6, 1e6, 6, voltageRfEdit));
    voltageRfEdit->setText("150");
    voltageRfUnitCombo = makeUnitCombo({"V", "mV"});
    auto* rfLabel = new QLabel("RF Voltage:");
    layout->addWidget(rfLabel, inputRow, 0);
    layout->addWidget(voltageRfEdit, inputRow, 1);
    layout->addWidget(voltageRfUnitCombo, inputRow++, 2);

    // RF Voltage (max)
    voltageRfMaxEdit = makeLineEdit("voltageRfMaxEdit");
    voltageRfMaxEdit->setValidator(new QDoubleValidator(-1e6, 1e6, 6, voltageRfMaxEdit));
    voltageRfMaxEdit->setText("3000");
    voltageRfMaxUnitCombo = makeUnitCombo({"V", "mV"});
    auto* rfMaxLabel = new QLabel("RF Voltage (max):");
    layout->addWidget(rfMaxLabel, inputRow, 0);
    layout->addWidget(voltageRfMaxEdit, inputRow, 1);
    layout->addWidget(voltageRfMaxUnitCombo, inputRow++, 2);

    // DC Voltage
    voltageDcEdit = makeLineEdit("voltageDcEdit");
    voltageDcEdit->setValidator(new QDoubleValidator(-1e6, 1e6, 6, voltageDcEdit));
    voltageDcEdit->setText("0");
    voltageDcUnitCombo = makeUnitCombo({"V", "mV"});
    auto* dcLabel = new QLabel("DC Voltage:");
    layout->addWidget(dcLabel, inputRow, 0);
    layout->addWidget(voltageDcEdit, inputRow, 1);
    layout->addWidget(voltageDcUnitCombo, inputRow++, 2);

    // Charge state
    chargeStateEdit = makeLineEdit("chargeStateEdit");
    chargeStateEdit->setValidator(new QIntValidator(1, 100, chargeStateEdit));
    chargeStateEdit->setText("1");
    auto* chargeLabel = new QLabel("Charge state:");
    layout->addWidget(chargeLabel, inputRow, 0);
    layout->addWidget(chargeStateEdit, inputRow, 1);

    setLayout(layout);
}

bool Inputs::validate() {
    bool ok_freq = false, ok_radius = false, ok_mass = false, ok_voltage_rf = false,
         ok_voltage_rf_max = false, ok_voltage_dc = false, ok_charge_state = false;
    frequencyEdit->text().toDouble(&ok_freq);
    radiusEdit->text().toDouble(&ok_radius);
    massEdit->text().toDouble(&ok_mass);
    voltageRfEdit->text().toDouble(&ok_voltage_rf);
    voltageRfMaxEdit->text().toDouble(&ok_voltage_rf_max);
    voltageDcEdit->text().toDouble(&ok_voltage_dc);
    chargeStateEdit->text().toInt(&ok_charge_state);
    bool allValid = ok_freq && ok_radius && ok_mass && ok_voltage_rf && ok_voltage_rf_max &&
                    ok_voltage_dc && ok_charge_state;
    frequencyEdit->setToolTip(ok_freq ? QStringLiteral("")
                                      : QStringLiteral("Enter a valid frequency (Hz)"));
    radiusEdit->setToolTip(ok_radius ? QStringLiteral("")
                                     : QStringLiteral("Enter a valid quadrupole radius (m)"));
    massEdit->setToolTip(ok_mass ? QStringLiteral("")
                                 : QStringLiteral("Enter a valid molar mass (kg/mol)"));
    voltageRfEdit->setToolTip(ok_voltage_rf ? QStringLiteral("")
                                            : QStringLiteral("Enter a valid RF voltage (V)"));
    voltageRfMaxEdit->setToolTip(ok_voltage_rf_max
                                     ? QStringLiteral("")
                                     : QStringLiteral("Enter a valid max RF voltage (V)"));
    voltageDcEdit->setToolTip(ok_voltage_dc ? QStringLiteral("")
                                            : QStringLiteral("Enter a valid DC voltage (V)"));
    chargeStateEdit->setToolTip(ok_charge_state
                                    ? QStringLiteral("")
                                    : QStringLiteral("Enter a valid charge state (integer)"));
    return allValid;
}

bool Inputs::getCalculationInputs(CalculationInputs& calcInputs) const {
    bool ok_freq = false, ok_radius = false, ok_mass = false, ok_voltage_rf = false,
         ok_voltage_rf_max = false, ok_voltage_dc = false, ok_charge_state = false;
    double freq = frequencyEdit->text().toDouble(&ok_freq);
    if (frequencyUnitCombo->currentText() == "kHz")
        freq *= 1000.0;
    double radius = radiusEdit->text().toDouble(&ok_radius);
    if (radiusUnitCombo->currentText() == "mm")
        radius /= 1000.0;
    double mass = massEdit->text().toDouble(&ok_mass);
    // mass is always in kg/mol, no conversion needed
    double voltage_rf = voltageRfEdit->text().toDouble(&ok_voltage_rf);
    if (voltageRfUnitCombo->currentText() == "mV")
        voltage_rf /= 1000.0;
    double voltage_rf_max = voltageRfMaxEdit->text().toDouble(&ok_voltage_rf_max);
    if (voltageRfMaxUnitCombo->currentText() == "mV")
        voltage_rf_max /= 1000.0;
    double voltage_dc = voltageDcEdit->text().toDouble(&ok_voltage_dc);
    if (voltageDcUnitCombo->currentText() == "mV")
        voltage_dc /= 1000.0;
    int charge_state = chargeStateEdit->text().toInt(&ok_charge_state);
    bool allValid = ok_freq && ok_radius && ok_mass && ok_voltage_rf && ok_voltage_rf_max &&
                    ok_voltage_dc && ok_charge_state;
    if (allValid) {
        calcInputs.freq = freq;
        calcInputs.radius = radius;
        calcInputs.mass = mass;
        calcInputs.voltage_rf = voltage_rf;
        calcInputs.voltage_rf_max = voltage_rf_max;
        calcInputs.voltage_dc = voltage_dc;
        calcInputs.charge_state = charge_state;
    }
    return allValid;
}
