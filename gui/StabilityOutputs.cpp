#include "StabilityOutputs.h"

StabilityOutputs::StabilityOutputs(QWidget* parent) : QWidget(parent) {
    auto* layout = new QGridLayout(this);
    QLabel* titleLabel = new QLabel("Stability Metrics", this);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel, 0, 0, 1, 3);

    QLabel* deltaANameLabel = new QLabel("Vertical distance to boundary:", this);
    deltaALabel = new QLabel("-", this);
    QLabel* deltaAUnitLabel = new QLabel("(Δa)", this);
    layout->addWidget(deltaANameLabel, 1, 0);
    layout->addWidget(deltaALabel, 1, 1);
    layout->addWidget(deltaAUnitLabel, 1, 2);

    QLabel* voltageDiffNameLabel = new QLabel("Voltage difference to boundary:", this);
    voltageDiffLabel = new QLabel("-", this);
    QLabel* voltageDiffUnitLabel = new QLabel("(V)", this);
    layout->addWidget(voltageDiffNameLabel, 2, 0);
    layout->addWidget(voltageDiffLabel, 2, 1);
    layout->addWidget(voltageDiffUnitLabel, 2, 2);

    QLabel* resolutionNameLabel = new QLabel("Resolution (Δa/a):", this);
    resolutionLabel = new QLabel("-", this);
    QLabel* resolutionUnitLabel = new QLabel("(unitless)", this);
    layout->addWidget(resolutionNameLabel, 3, 0);
    layout->addWidget(resolutionLabel, 3, 1);
    layout->addWidget(resolutionUnitLabel, 3, 2);

    setLayout(layout);
    setMaximumWidth(300);  // Limit to half the width of a typical graph (adjust as needed)
}

void StabilityOutputs::setDeltaA(double value) {
    deltaALabel->setText(QString::number(value, 'f', 3));
}

void StabilityOutputs::setVoltageDiff(double value) {
    voltageDiffLabel->setText(QString::number(value, 'f', 3));
}

void StabilityOutputs::setResolution(double value) {
    resolutionLabel->setText(QString::number(value, 'f', 3));
}
