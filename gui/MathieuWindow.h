#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QWidget>
#include <memory>

#include "Inputs.h"
#include "MiniCalculator.h"
#include "Outputs.h"
#include "plot/StabilityRegionPlotter.h"
#include "stability/StabilityOutputs.h"

namespace trappable {

class MathieuWindow : public QWidget {
   public:
    explicit MathieuWindow(QWidget* parent = nullptr);
    ~MathieuWindow() override;

   public:
    void triggerMiniCalculator();
    QPushButton* calcButton;
    class Inputs* inputs;
    class Outputs* outputs;

    QCustomPlot* stabilityPlotWidget;
    StabilityRegionPlotter* stabilityPlotter;

    // Stability outputs component
    StabilityOutputs* stabilityOutputs;

    // MiniCalculator applet
    MiniCalculator* miniCalculator;

   private:
    void validateInputs();
    void handleCalculation();
    void setOutputInvalid();
    void setOutputValues(double omega_val, double particle_mass_val, double mathieu_q_val,
                         double mathieu_a_val, double beta_val, double secular_freq_val,
                         double mz_val, double lmco_val, double max_mz_val);
};

}  // namespace trappable
