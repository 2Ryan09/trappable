#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QtTest/QtTest>

#include "MathieuWindow.h"

class GuiE2ETest : public QObject {
   public:
    void test_all_inputs_and_outputs();
};

void GuiE2ETest::test_all_inputs_and_outputs() {
    // Create the main window
    MathieuWindow w;
    w.show();

    // Set all input values (replace with your test values)
    w.frequencyEdit->setText("970000");
    w.radiusEdit->setText("0.003478");
    w.massEdit->setText("0.303");
    w.voltageRfEdit->setText("150");
    w.voltageRfMaxEdit->setText("3000");
    w.voltageDcEdit->setText("0");
    w.chargeStateEdit->setText("1");

    // Simulate button click
    QTest::mouseClick(w.calcButton, Qt::LeftButton);

    // Extract and compare individual values
    QString output = w.resultLabel->text();

    QRegularExpression re_omega("omega: ([0-9.eE+-]+)");
    QRegularExpression re_mass("particle_mass: ([0-9.eE+-]+)");
    QRegularExpression re_q("mathieu_q: ([0-9.eE+-]+)");
    QRegularExpression re_a("mathieu_a: ([0-9.eE+-]+)");
    QRegularExpression re_beta("beta: ([0-9.eE+-]+)");
    QRegularExpression re_secular("secular_frequency: ([0-9.eE+-]+)");
    QRegularExpression re_mz("m/z: ([0-9.eE+-]+)");
    QRegularExpression re_lmco("lmco: ([0-9.eE+-]+)");
    QRegularExpression re_maxmz("max_mz: ([0-9.eE+-]+)");

    auto extract = [](const QString &text, const QRegularExpression &re) -> double {
        auto match = re.match(text);
        if (match.hasMatch()) {
            return match.captured(1).toDouble();
        }
        return std::numeric_limits<double>::quiet_NaN();
    };

    double omega = extract(output, re_omega);
    double particle_mass = extract(output, re_mass);
    double mathieu_q = extract(output, re_q);
    double mathieu_a = extract(output, re_a);
    double beta = extract(output, re_beta);
    double secular_frequency = extract(output, re_secular);
    double mz = extract(output, re_mz);
    double lmco = extract(output, re_lmco);
    double max_mz = extract(output, re_maxmz);

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
