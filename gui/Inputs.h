#ifndef INPUTS_H
#define INPUTS_H

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QWidget>

class Inputs : public QWidget {
    Q_OBJECT
   public:
    explicit Inputs(QWidget* parent = nullptr);
    bool validate();

    struct CalculationInputs {
        double freq;
        double radius;
        double mass;
        double voltage_rf;
        double voltage_rf_max;
        double voltage_dc;
        int charge_state;
    };

    // Returns true if all values are valid and fills calcInputs with converted values
    bool getCalculationInputs(CalculationInputs& calcInputs) const;
    QLineEdit* frequencyEdit;
    QComboBox* frequencyUnitCombo;
    QLineEdit* radiusEdit;
    QComboBox* radiusUnitCombo;
    QLineEdit* massEdit;
    QLabel* massUnitLabel;
    QLineEdit* voltageRfEdit;
    QComboBox* voltageRfUnitCombo;
    QLineEdit* voltageRfMaxEdit;
    QComboBox* voltageRfMaxUnitCombo;
    QLineEdit* voltageDcEdit;
    QComboBox* voltageDcUnitCombo;
    QLineEdit* chargeStateEdit;
};

#endif  // INPUTS_H
