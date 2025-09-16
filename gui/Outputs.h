#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QWidget>

class Outputs : public QWidget {
    Q_OBJECT
   public:
    explicit Outputs(QWidget* parent = nullptr);
    void setInvalid();
    void setValues(double omega_val, double particle_mass_val, double mathieu_q_val,
                   double mathieu_a_val, double beta_val, double secular_freq_val, double mz_val,
                   double lmco_val, double max_mz_val);
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
};

#endif  // OUTPUTS_H
