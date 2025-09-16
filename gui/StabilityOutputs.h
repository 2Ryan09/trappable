#ifndef STABILITYOUTPUTS_H
#define STABILITYOUTPUTS_H

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

class StabilityOutputs : public QWidget {
    Q_OBJECT
   public:
    explicit StabilityOutputs(QWidget* parent = nullptr);
    void setDeltaA(double value);
    void setVoltageDiff(double value);
    void setResolution(double value);

   private:
    QLabel* deltaALabel;
    QLabel* voltageDiffLabel;
    QLabel* resolutionLabel;
};

#endif  // STABILITYOUTPUTS_H
