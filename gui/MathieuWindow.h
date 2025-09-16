#pragma once
#include <QWidget>
#include <memory>

class QLineEdit;
class QPushButton;
class QLabel;

#include "plot/StabilityRegionPlotter.h"

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
};
