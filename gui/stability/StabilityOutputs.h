#ifndef STABILITYOUTPUTS_H
#define STABILITYOUTPUTS_H

#include <QGraphicsBlurEffect>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

class StabilityOutputs : public QWidget {
    Q_OBJECT
   protected:
    void resizeEvent(QResizeEvent* event) override;

   public:
    void setWarning(const QString& message);
    void clearWarning();
    explicit StabilityOutputs(QWidget* parent = nullptr);
    ~StabilityOutputs();
    void setDeltaA(double value);
    void setVoltageDiff(double value);
    void setResolution(double value);

    // New advanced metrics
    void setDeltaQ(double value);
    void setDeltaE(double value);
    void setTheta(double value);
    void setSNorm(double value);
    void setDeltaMin(double value);

    // Public getters for testing
    QLabel* getOverlayLabel() const { return overlayLabel; }
    QWidget* getWarningOverlay() const { return warningOverlay; }
    QLabel* getDeltaALabel() const { return deltaALabel; }
    QLabel* getDeltaQLabel() const { return deltaQLabel; }
    QLabel* getDeltaELabel() const { return deltaELabel; }
    QLabel* getThetaLabel() const { return thetaLabel; }
    QLabel* getSNormLabel() const { return sNormLabel; }
    QLabel* getDeltaMinLabel() const { return deltaMinLabel; }
    QLabel* getVoltageDiffLabel() const { return voltageDiffLabel; }
    QLabel* getResolutionLabel() const { return resolutionLabel; }

   private:
    QWidget* warningOverlay;  // Frosted glass overlay
    QLabel* overlayLabel;     // Centered warning message on overlay
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
