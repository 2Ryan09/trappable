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

    // Check output contains all expected values (replace with your expected values)
    QString output = w.resultLabel->text();
    QVERIFY(output.contains("omega:"));
    QVERIFY(output.contains("particle_mass:"));
    QVERIFY(output.contains("mathieu_q:"));
    QVERIFY(output.contains("mathieu_a:"));
    QVERIFY(output.contains("beta:"));
    QVERIFY(output.contains("secular_frequency:"));
    QVERIFY(output.contains("m/z:"));
    QVERIFY(output.contains("lmco:"));
    QVERIFY(output.contains("max_mz:"));
    // Add QCOMPARE(output, expected_output) when you provide exact values
}

QTEST_MAIN(GuiE2ETest)
