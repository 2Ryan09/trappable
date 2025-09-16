#pragma once
#include <QWidget>
#include <memory>

class QLineEdit;
class QPushButton;
class QLabel;

#include "plot/StabilityRegionPlotter.h"

namespace trappable {

class MathieuWindow : public QWidget {
   public:
    explicit MathieuWindow(QWidget* parent = nullptr);
    ~MathieuWindow() override;

    QLineEdit* frequencyEdit;
    QLineEdit* radiusEdit;
    QLineEdit* massEdit;
    QLineEdit* voltageRfEdit;
    QLineEdit* voltageRfMaxEdit;
    QLineEdit* voltageDcEdit;
    QLineEdit* chargeStateEdit;
    QPushButton* calcButton;
    QLabel* omegaValueLabel;
    QLabel* omegaUnitLabel;
    QLabel* particleMassValueLabel;
    QLabel* particleMassUnitLabel;
    QLabel* mathieuQValueLabel;
    QLabel* mathieuQUnitLabel;
    QLabel* mathieuAValueLabel;
    QLabel* mathieuAUnitLabel;
    QLabel* betaValueLabel;
    QLabel* betaUnitLabel;
    QLabel* secularFrequencyValueLabel;
    QLabel* secularFrequencyUnitLabel;
    QLabel* mzValueLabel;
    QLabel* mzUnitLabel;
    QLabel* lmcoValueLabel;
    QLabel* lmcoUnitLabel;
    QLabel* maxMzValueLabel;
    QLabel* maxMzUnitLabel;

    QCustomPlot* stabilityPlotWidget;
    StabilityRegionPlotter* stabilityPlotter;

   private:
    void validateInputs();
    void handleCalculation();
    void setOutputInvalid();
    void setOutputValues(double omega_val, double particle_mass_val, double mathieu_q_val,
                         double mathieu_a_val, double beta_val, double secular_freq_val,
                         double mz_val, double lmco_val, double max_mz_val);
};

}  // namespace trappable
