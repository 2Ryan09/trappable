#include "StabilityOutputs.h"

StabilityOutputs::StabilityOutputs(QWidget* parent) : QWidget(parent) {
    auto* layout = new QGridLayout(this);
    // Metrics title
    QLabel* titleLabel = new QLabel("Stability Metrics", this);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel, 0, 0, 1, 3);

    // Frosted glass warning overlay (hidden by default)
    warningOverlay = new QWidget(this);
    warningOverlay->setVisible(false);
    warningOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Create a background widget for blur
    QWidget* blurBg = new QWidget(warningOverlay);
    blurBg->setStyleSheet("background: rgba(255,255,255,0.7);");
    QGraphicsBlurEffect* blur = new QGraphicsBlurEffect(blurBg);
    blur->setBlurRadius(32);
    blurBg->setGraphicsEffect(blur);

    // Overlay label (no blur)
    overlayLabel = new QLabel(warningOverlay);
    overlayLabel->setAlignment(Qt::AlignCenter);
    QFont overlayFont;
    overlayFont.setBold(true);
    overlayFont.setPointSize(20);
    overlayLabel->setFont(overlayFont);
    overlayLabel->setStyleSheet("color: red;");
    overlayLabel->setWordWrap(true);
    overlayLabel->setText("Ion Unstable");

    QLabel* deltaANameLabel = new QLabel("Vertical distance to boundary:", this);
    deltaALabel = new QLabel("-", this);
    deltaALabel->setAlignment(Qt::AlignRight);
    QLabel* deltaAUnitLabel = new QLabel("Δa", this);
    layout->addWidget(deltaANameLabel, 1, 0);
    layout->addWidget(deltaALabel, 1, 1);
    layout->addWidget(deltaAUnitLabel, 1, 2);

    QLabel* deltaQNameLabel = new QLabel("Horizontal distance to boundary:", this);
    deltaQLabel = new QLabel("-", this);
    deltaQLabel->setAlignment(Qt::AlignRight);
    QLabel* deltaQUnitLabel = new QLabel("Δq", this);
    layout->addWidget(deltaQNameLabel, 2, 0);
    layout->addWidget(deltaQLabel, 2, 1);
    layout->addWidget(deltaQUnitLabel, 2, 2);

    QLabel* deltaENameLabel = new QLabel("Euclidean distance to boundary:", this);
    deltaELabel = new QLabel("-", this);
    deltaELabel->setAlignment(Qt::AlignRight);
    QLabel* deltaEUnitLabel = new QLabel("Δₑ", this);
    layout->addWidget(deltaENameLabel, 3, 0);
    layout->addWidget(deltaELabel, 3, 1);
    layout->addWidget(deltaEUnitLabel, 3, 2);

    QLabel* thetaNameLabel = new QLabel("Angular offset to boundary:", this);
    thetaLabel = new QLabel("-", this);
    thetaLabel->setAlignment(Qt::AlignRight);
    QLabel* thetaUnitLabel = new QLabel("deg", this);
    layout->addWidget(thetaNameLabel, 4, 0);
    layout->addWidget(thetaLabel, 4, 1);
    layout->addWidget(thetaUnitLabel, 4, 2);

    QLabel* sNormNameLabel = new QLabel("Normalized stability margin:", this);
    sNormLabel = new QLabel("-", this);
    sNormLabel->setAlignment(Qt::AlignRight);
    QLabel* sNormUnitLabel = new QLabel("", this);
    layout->addWidget(sNormNameLabel, 5, 0);
    layout->addWidget(sNormLabel, 5, 1);
    layout->addWidget(sNormUnitLabel, 5, 2);

    QLabel* deltaMinNameLabel = new QLabel("Worst-case boundary proximity:", this);
    deltaMinLabel = new QLabel("-", this);
    deltaMinLabel->setAlignment(Qt::AlignRight);
    QLabel* deltaMinUnitLabel = new QLabel("Δmin", this);
    layout->addWidget(deltaMinNameLabel, 6, 0);
    layout->addWidget(deltaMinLabel, 6, 1);
    layout->addWidget(deltaMinUnitLabel, 6, 2);

    QLabel* voltageDiffNameLabel = new QLabel("Voltage difference to boundary:", this);
    voltageDiffLabel = new QLabel("-", this);
    voltageDiffLabel->setAlignment(Qt::AlignRight);
    QLabel* voltageDiffUnitLabel = new QLabel("V", this);
    layout->addWidget(voltageDiffNameLabel, 7, 0);
    layout->addWidget(voltageDiffLabel, 7, 1);
    layout->addWidget(voltageDiffUnitLabel, 7, 2);

    QLabel* resolutionNameLabel = new QLabel("Resolution (Δa/a):", this);
    resolutionLabel = new QLabel("-", this);
    resolutionLabel->setAlignment(Qt::AlignRight);
    QLabel* resolutionUnitLabel = new QLabel("", this);
    layout->addWidget(resolutionNameLabel, 8, 0);
    layout->addWidget(resolutionLabel, 8, 1);
    layout->addWidget(resolutionUnitLabel, 8, 2);

    setLayout(layout);
}

void StabilityOutputs::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    if (warningOverlay) {
        warningOverlay->raise();  // Ensure overlay is always on top
        warningOverlay->setGeometry(rect());
        // Resize blur background and label to fill overlay
        QList<QWidget*> children = warningOverlay->findChildren<QWidget*>();
        for (QWidget* child : children) {
            child->setGeometry(warningOverlay->rect());
        }
    }
}

void StabilityOutputs::setWarning(const QString&) {
    overlayLabel->setText("Ion Unstable");
    warningOverlay->setVisible(true);
    // Set all stability output labels to "-"
    deltaALabel->setText("-");
    deltaQLabel->setText("-");
    deltaELabel->setText("-");
    thetaLabel->setText("-");
    sNormLabel->setText("-");
    deltaMinLabel->setText("-");
    voltageDiffLabel->setText("-");
    resolutionLabel->setText("-");
}

void StabilityOutputs::clearWarning() {
    overlayLabel->clear();
    warningOverlay->setVisible(false);
}

void StabilityOutputs::setDeltaA(double value) {
    deltaALabel->setText(QString::number(value, 'f', 3));
}

void StabilityOutputs::setDeltaQ(double value) {
    deltaQLabel->setText(QString::number(value, 'f', 3));
}

void StabilityOutputs::setDeltaE(double value) {
    deltaELabel->setText(QString::number(value, 'f', 3));
}

void StabilityOutputs::setTheta(double value) {
    thetaLabel->setText(QString::number(value, 'f', 2));
}

void StabilityOutputs::setSNorm(double value) {
    sNormLabel->setText(QString::number(value, 'f', 3));
}

void StabilityOutputs::setDeltaMin(double value) {
    deltaMinLabel->setText(QString::number(value, 'f', 3));
}

void StabilityOutputs::setVoltageDiff(double value) {
    voltageDiffLabel->setText(QString::number(value, 'f', 3));
}

void StabilityOutputs::setResolution(double value) {
    resolutionLabel->setText(QString::number(value, 'f', 3));
}
