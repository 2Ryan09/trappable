#ifndef INPUTS_H
#define INPUTS_H

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
    QRadioButton* frequencyUnitHz;
    QRadioButton* frequencyUnitKHz;
    QLineEdit* radiusEdit;
    QRadioButton* radiusUnitM;
    QRadioButton* radiusUnitMM;
    QLineEdit* massEdit;
    QRadioButton* massUnitKg;
    QRadioButton* massUnitG;
    QLineEdit* voltageRfEdit;
    QRadioButton* voltageRfUnitV;
    QRadioButton* voltageRfUnitMV;
    QLineEdit* voltageRfMaxEdit;
    QRadioButton* voltageRfMaxUnitV;
    QRadioButton* voltageRfMaxUnitMV;
    QLineEdit* voltageDcEdit;
    QRadioButton* voltageDcUnitV;
    QRadioButton* voltageDcUnitMV;
    QLineEdit* chargeStateEdit;
};

#endif  // INPUTS_H
