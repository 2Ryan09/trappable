#include <gtest/gtest.h>

#include <QApplication>

#include "MathieuWindow.h"

using namespace trappable;

// Helper to create QApplication for widget tests
static int argc = 0;
static char* argv[] = {nullptr};
static QApplication app(argc, argv);

class MathieuWindowTest : public ::testing::Test {
   protected:
    MathieuWindow* window;
    void SetUp() override {
        window = new MathieuWindow(nullptr);
        window->show();
    }
    void TearDown() override { delete window; }
};

TEST_F(MathieuWindowTest, InitialState) {
    EXPECT_FALSE(window->calcButton->isEnabled());
    EXPECT_EQ(window->omegaValueLabel->text(), "");
    EXPECT_EQ(window->particleMassValueLabel->text(), "");
}

TEST_F(MathieuWindowTest, InputValidationEnablesButton) {
    window->frequencyEdit->setText("1000000");
    window->radiusEdit->setText("0.01");
    window->massEdit->setText("1.0");
    window->voltageRfEdit->setText("1000");
    window->voltageRfMaxEdit->setText("2000");
    window->voltageDcEdit->setText("0");
    window->chargeStateEdit->setText("1");
    // Simulate Qt event loop
    QApplication::processEvents();
    EXPECT_TRUE(window->calcButton->isEnabled());
}

TEST_F(MathieuWindowTest, InvalidInputDisablesButton) {
    window->frequencyEdit->setText("");
    window->radiusEdit->setText("0.01");
    window->massEdit->setText("1.0");
    window->voltageRfEdit->setText("1000");
    window->voltageRfMaxEdit->setText("2000");
    window->voltageDcEdit->setText("0");
    window->chargeStateEdit->setText("1");
    QApplication::processEvents();
    EXPECT_FALSE(window->calcButton->isEnabled());
}

TEST_F(MathieuWindowTest, CalculationUpdatesOutputs) {
    window->frequencyEdit->setText("1000000");
    window->radiusEdit->setText("0.01");
    window->massEdit->setText("1.0");
    window->voltageRfEdit->setText("1000");
    window->voltageRfMaxEdit->setText("2000");
    window->voltageDcEdit->setText("0");
    window->chargeStateEdit->setText("1");
    QApplication::processEvents();
    ASSERT_TRUE(window->calcButton->isEnabled());
    // Simulate button click
    QMetaObject::invokeMethod(window->calcButton, "click");
    QApplication::processEvents();
    EXPECT_NE(window->omegaValueLabel->text(), "");
    EXPECT_NE(window->particleMassValueLabel->text(), "");
    EXPECT_NE(window->mathieuQValueLabel->text(), "");
    EXPECT_NE(window->mathieuAValueLabel->text(), "");
    EXPECT_NE(window->betaValueLabel->text(), "");
    EXPECT_NE(window->secularFrequencyValueLabel->text(), "");
    EXPECT_NE(window->mzValueLabel->text(), "");
    EXPECT_NE(window->lmcoValueLabel->text(), "");
    EXPECT_NE(window->maxMzValueLabel->text(), "");
}

TEST_F(MathieuWindowTest, OutputInvalidOnBadInput) {
    window->frequencyEdit->setText("");
    window->radiusEdit->setText("");
    window->massEdit->setText("");
    window->voltageRfEdit->setText("");
    window->voltageRfMaxEdit->setText("");
    window->voltageDcEdit->setText("");
    window->chargeStateEdit->setText("");
    QApplication::processEvents();
    // Button should be disabled, so calculation cannot be triggered
    EXPECT_FALSE(window->calcButton->isEnabled());
    EXPECT_EQ(window->omegaValueLabel->text(), "");
    EXPECT_EQ(window->particleMassValueLabel->text(), "");
    EXPECT_EQ(window->mathieuQValueLabel->text(), "");
    EXPECT_EQ(window->mathieuAValueLabel->text(), "");
    EXPECT_EQ(window->betaValueLabel->text(), "");
    EXPECT_EQ(window->secularFrequencyValueLabel->text(), "");
    EXPECT_EQ(window->mzValueLabel->text(), "");
    EXPECT_EQ(window->lmcoValueLabel->text(), "");
    EXPECT_EQ(window->maxMzValueLabel->text(), "");
}

TEST_F(MathieuWindowTest, EdgeCaseInputs) {
    // Extreme values
    window->frequencyEdit->setText("1e12");
    window->radiusEdit->setText("1e-6");
    window->massEdit->setText("1e-30");
    window->voltageRfEdit->setText("1e6");
    window->voltageRfMaxEdit->setText("1e7");
    window->voltageDcEdit->setText("-1e6");
    window->chargeStateEdit->setText("99");
    QApplication::processEvents();
    EXPECT_TRUE(window->calcButton->isEnabled());
    QMetaObject::invokeMethod(window->calcButton, "click");
    QApplication::processEvents();
    // Outputs should be non-empty and formatted
    EXPECT_NE(window->omegaValueLabel->text(), "");
    EXPECT_NE(window->particleMassValueLabel->text(), "");
    EXPECT_NE(window->mathieuQValueLabel->text(), "");
    EXPECT_NE(window->mathieuAValueLabel->text(), "");
    EXPECT_NE(window->betaValueLabel->text(), "");
    EXPECT_NE(window->secularFrequencyValueLabel->text(), "");
    EXPECT_NE(window->mzValueLabel->text(), "");
    EXPECT_NE(window->lmcoValueLabel->text(), "");
    EXPECT_NE(window->maxMzValueLabel->text(), "");
}

TEST_F(MathieuWindowTest, PlotWidgetUpdates) {
    window->frequencyEdit->setText("1000000");
    window->radiusEdit->setText("0.01");
    window->massEdit->setText("1.0");
    window->voltageRfEdit->setText("1000");
    window->voltageRfMaxEdit->setText("2000");
    window->voltageDcEdit->setText("0");
    window->chargeStateEdit->setText("1");
    QApplication::processEvents();
    ASSERT_TRUE(window->calcButton->isEnabled());
    // Before calculation, plot should have no user points
    int initialGraphs = window->stabilityPlotWidget->graphCount();
    QMetaObject::invokeMethod(window->calcButton, "click");
    QApplication::processEvents();
    // After calculation, plot should have at least one more graph (the point)
    int afterGraphs = window->stabilityPlotWidget->graphCount();
    EXPECT_GT(afterGraphs, initialGraphs);
}

int main(int argc, char** argv) {
    // QApplication already constructed above
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
