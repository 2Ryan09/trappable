#include <gtest/gtest.h>

#include <QApplication>

#include "stability/StabilityOutputs.h"

// Helper to create QApplication for widget tests
static int argc = 0;
static char* argv[] = {nullptr};
static QApplication app(argc, argv);

TEST(StabilityOutputsTest, OverlayAppearsOnUnstable) {
    StabilityOutputs outputs;
    outputs.show();
    outputs.setWarning("");
    QApplication::processEvents();
    EXPECT_TRUE(outputs.getWarningOverlay()->isVisible());
    EXPECT_EQ(outputs.getOverlayLabel()->text(), "Ion Unstable");
}

TEST(StabilityOutputsTest, MetricsSuppressedOnUnstable) {
    StabilityOutputs outputs;
    outputs.setWarning("");
    EXPECT_EQ(outputs.getDeltaALabel()->text(), "-");
    EXPECT_EQ(outputs.getDeltaQLabel()->text(), "-");
    EXPECT_EQ(outputs.getDeltaELabel()->text(), "-");
    EXPECT_EQ(outputs.getThetaLabel()->text(), "-");
    EXPECT_EQ(outputs.getSNormLabel()->text(), "-");
    EXPECT_EQ(outputs.getDeltaMinLabel()->text(), "-");
    EXPECT_EQ(outputs.getVoltageDiffLabel()->text(), "-");
    EXPECT_EQ(outputs.getResolutionLabel()->text(), "-");
}

TEST(StabilityOutputsTest, OverlayTextNotBlurred) {
    StabilityOutputs outputs;
    outputs.setWarning("");
    // The overlayLabel should not have a QGraphicsBlurEffect
    EXPECT_EQ(outputs.getOverlayLabel()->graphicsEffect(), nullptr);
}

TEST(StabilityOutputsTest, OverlayBlurRadius) {
    StabilityOutputs outputs;
    outputs.setWarning("");
    QWidget* blurBg =
        outputs.getWarningOverlay()->findChild<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
    auto* blurEffect = dynamic_cast<QGraphicsBlurEffect*>(blurBg->graphicsEffect());
    ASSERT_NE(blurEffect, nullptr);
    EXPECT_GE(blurEffect->blurRadius(), 32);
}
