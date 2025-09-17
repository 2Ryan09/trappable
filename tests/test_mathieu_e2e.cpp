#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QtTest/QtTest>

#include "MathieuWindow.h"
using trappable::MathieuWindow;

class GuiE2ETest : public QObject {
   public:
    void test_all_inputs_and_outputs();
};

void GuiE2ETest::test_all_inputs_and_outputs() {
    // Create the main window
    MathieuWindow w;
    w.show();

    // Set all input values (replace with your test values)
    w.inputs->frequencyEdit->setText("970000");
    w.inputs->radiusEdit->setText("0.003478");
    w.inputs->massEdit->setText("0.303");
    w.inputs->voltageRfEdit->setText("150");
    w.inputs->voltageRfMaxEdit->setText("3000");
    w.inputs->voltageDcEdit->setText("0");
    w.inputs->chargeStateEdit->setText("1");
    // Set units explicitly for robustness (using combo boxes)
    w.inputs->frequencyUnitCombo->setCurrentText("Hz");
    w.inputs->radiusUnitCombo->setCurrentText("m");
    w.inputs->voltageRfUnitCombo->setCurrentText("V");
    w.inputs->voltageRfMaxUnitCombo->setCurrentText("V");
    w.inputs->voltageDcUnitCombo->setCurrentText("V");

    // Simulate button click
    QTest::mouseClick(w.calcButton, Qt::LeftButton);

    // Extract and compare individual values from output labels
    auto extract = [](const QString &text) -> double { return text.toDouble(); };

    double omega = extract(w.outputs->omegaValueLabel->text());
    double particle_mass = extract(w.outputs->particleMassValueLabel->text());
    double mathieu_q = extract(w.outputs->mathieuQValueLabel->text());
    double mathieu_a = extract(w.outputs->mathieuAValueLabel->text());
    double beta = extract(w.outputs->betaValueLabel->text());
    double secular_frequency = extract(w.outputs->secularFrequencyValueLabel->text());
    double mz = extract(w.outputs->mzValueLabel->text());
    double lmco = extract(w.outputs->lmcoValueLabel->text());
    double max_mz = extract(w.outputs->maxMzValueLabel->text());

    // Replace these with your established expected values
    double expected_omega = 6.095e6;
    double expected_particle_mass = 5.031e-25;
    double expected_mathieu_q = 0.213;
    double expected_mathieu_a = 0.0;
    double expected_beta = 0.150;
    double expected_secular_frequency = 72.913;
    double expected_mz = 303;
    double expected_lmco = 70.947;
    double expected_max_mz = 1418.94;

    QVERIFY(!std::isnan(omega));
    QVERIFY(!std::isnan(particle_mass));
    QVERIFY(!std::isnan(mathieu_q));
    QVERIFY(!std::isnan(mathieu_a));
    QVERIFY(!std::isnan(beta));
    QVERIFY(!std::isnan(secular_frequency));
    QVERIFY(!std::isnan(mz));
    QVERIFY(!std::isnan(lmco));
    QVERIFY(!std::isnan(max_mz));

    QCOMPARE(omega, expected_omega);
    QCOMPARE(particle_mass, expected_particle_mass);
    QCOMPARE(mathieu_q, expected_mathieu_q);
    QCOMPARE(mathieu_a, expected_mathieu_a);
    QCOMPARE(beta, expected_beta);
    QCOMPARE(secular_frequency, expected_secular_frequency);
    QCOMPARE(mz, expected_mz);
    QCOMPARE(lmco, expected_lmco);
    QCOMPARE(max_mz, expected_max_mz);
}

QTEST_MAIN(GuiE2ETest)
