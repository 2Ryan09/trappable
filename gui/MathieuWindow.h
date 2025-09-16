#pragma once
#include <QWidget>
#include <memory>

class QLineEdit;
class QPushButton;
class QLabel;

#include "plot/MathieuStabilityPlot.h"

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
    QLabel* resultLabel;

    MathieuStabilityPlot* stabilityPlot;
};
