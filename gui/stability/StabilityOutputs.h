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

    // New advanced metrics
    void setDeltaQ(double value);
    void setDeltaE(double value);
    void setTheta(double value);
    void setSNorm(double value);
    void setDeltaMin(double value);

   private:
    QLabel* deltaALabel;
    QLabel* voltageDiffLabel;
    QLabel* resolutionLabel;

    QLabel* deltaQLabel;
    QLabel* deltaELabel;
    QLabel* thetaLabel;
    QLabel* sNormLabel;
    QLabel* deltaMinLabel;
};

#endif  // STABILITYOUTPUTS_H
