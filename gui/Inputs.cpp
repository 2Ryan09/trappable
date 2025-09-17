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

    frequencyEdit = makeLineEdit("frequencyEdit");
    frequencyEdit->setValidator(new QDoubleValidator(0, 1e12, 6, frequencyEdit));
    frequencyEdit->setText("970000");  // Default frequency in Hz
    auto [frequencyUnitHz, frequencyUnitKHz, frequencyUnitWidget] = makeUnitRadios("Hz", "kHz");
    this->frequencyUnitHz = frequencyUnitHz;
    this->frequencyUnitKHz = frequencyUnitKHz;

    radiusEdit = makeLineEdit("radiusEdit");
    radiusEdit->setValidator(new QDoubleValidator(0, 1e6, 6, radiusEdit));
    radiusEdit->setText("0.003478");  // Default radius in m
    auto [radiusUnitM, radiusUnitMM, radiusUnitWidget] = makeUnitRadios("m", "mm");
    this->radiusUnitM = radiusUnitM;
    this->radiusUnitMM = radiusUnitMM;

    massEdit = makeLineEdit("massEdit");
    massEdit->setValidator(new QDoubleValidator(0, 1e6, 6, massEdit));
    massEdit->setText("0.303");  // Default mass in kg
    auto [massUnitKg, massUnitG, massUnitWidget] = makeUnitRadios("kg", "g");
    this->massUnitKg = massUnitKg;
    this->massUnitG = massUnitG;

    voltageRfEdit = makeLineEdit("voltageRfEdit");
    voltageRfEdit->setValidator(new QDoubleValidator(-1e6, 1e6, 6, voltageRfEdit));
    voltageRfEdit->setText("150");  // Default RF voltage in V
    auto [voltageRfUnitV, voltageRfUnitMV, voltageRfUnitWidget] = makeUnitRadios("V", "mV");
    this->voltageRfUnitV = voltageRfUnitV;
    this->voltageRfUnitMV = voltageRfUnitMV;

    voltageRfMaxEdit = makeLineEdit("voltageRfMaxEdit");
    voltageRfMaxEdit->setValidator(new QDoubleValidator(-1e6, 1e6, 6, voltageRfMaxEdit));
    voltageRfMaxEdit->setText("3000");  // Default max RF voltage in V
    auto [voltageRfMaxUnitV, voltageRfMaxUnitMV, voltageRfMaxUnitWidget] =
        makeUnitRadios("V", "mV");
    this->voltageRfMaxUnitV = voltageRfMaxUnitV;
    this->voltageRfMaxUnitMV = voltageRfMaxUnitMV;

    voltageDcEdit = makeLineEdit("voltageDcEdit");
    voltageDcEdit->setValidator(new QDoubleValidator(-1e6, 1e6, 6, voltageDcEdit));
    voltageDcEdit->setText("0");  // Default DC voltage in V
    auto [voltageDcUnitV, voltageDcUnitMV, voltageDcUnitWidget] = makeUnitRadios("V", "mV");
    this->voltageDcUnitV = voltageDcUnitV;
    this->voltageDcUnitMV = voltageDcUnitMV;

    chargeStateEdit = makeLineEdit("chargeStateEdit");
    chargeStateEdit->setValidator(new QIntValidator(1, 100, chargeStateEdit));
    chargeStateEdit->setText("1");  // Default charge state

    int inputRow = 0;
    auto* freqLabel = new QLabel("Frequency:");
    layout->addWidget(freqLabel, inputRow, 0);
    layout->addWidget(frequencyEdit, inputRow, 1);
    layout->addWidget(frequencyUnitWidget, inputRow++, 2);
    auto* radiusLabel = new QLabel("Quadrupole radius:");
    layout->addWidget(radiusLabel, inputRow, 0);
    layout->addWidget(radiusEdit, inputRow, 1);
    layout->addWidget(radiusUnitWidget, inputRow++, 2);
    auto* massLabel = new QLabel("Molar mass:");
    layout->addWidget(massLabel, inputRow, 0);
    layout->addWidget(massEdit, inputRow, 1);
    layout->addWidget(massUnitWidget, inputRow++, 2);
    auto* rfLabel = new QLabel("RF Voltage:");
    layout->addWidget(rfLabel, inputRow, 0);
    layout->addWidget(voltageRfEdit, inputRow, 1);
    layout->addWidget(voltageRfUnitWidget, inputRow++, 2);
    auto* rfMaxLabel = new QLabel("RF Voltage (max):");
    layout->addWidget(rfMaxLabel, inputRow, 0);
    layout->addWidget(voltageRfMaxEdit, inputRow, 1);
    layout->addWidget(voltageRfMaxUnitWidget, inputRow++, 2);
    auto* dcLabel = new QLabel("DC Voltage:");
    layout->addWidget(dcLabel, inputRow, 0);
    layout->addWidget(voltageDcEdit, inputRow, 1);
    layout->addWidget(voltageDcUnitWidget, inputRow++, 2);
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
    if (frequencyUnitKHz->isChecked())
        freq *= 1000.0;
    double radius = radiusEdit->text().toDouble(&ok_radius);
    if (radiusUnitMM->isChecked())
        radius /= 1000.0;
    double mass = massEdit->text().toDouble(&ok_mass);
    if (massUnitG->isChecked())
        mass /= 1000.0;
    double voltage_rf = voltageRfEdit->text().toDouble(&ok_voltage_rf);
    if (voltageRfUnitMV->isChecked())
        voltage_rf /= 1000.0;
    double voltage_rf_max = voltageRfMaxEdit->text().toDouble(&ok_voltage_rf_max);
    if (voltageRfMaxUnitMV->isChecked())
        voltage_rf_max /= 1000.0;
    double voltage_dc = voltageDcEdit->text().toDouble(&ok_voltage_dc);
    if (voltageDcUnitMV->isChecked())
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
