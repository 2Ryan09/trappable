#include "MiniCalculator.h"

#include <QString>
#include <cmath>

#include "Constants.h"
#include "mathieu_lib/mathieu.h"

MiniCalculator::MiniCalculator(QWidget* parent) : QWidget(parent) {
    auto* layout = new QGridLayout(this);
    mzLabel = new QLabel("m/z:", this);
    mzEdit = new QLineEdit(this);
    mzEdit->setText("");
    hrLine = new QFrame(this);
    hrLine->setFrameShape(QFrame::HLine);
    hrLine->setFrameShadow(QFrame::Sunken);
    qLabel = new QLabel("Mathieu q:", this);
    qEdit = new FocusClearLineEdit(nullptr, this);
    vLabel = new QLabel("V trapping:", this);
    vEdit = new FocusClearLineEdit(qEdit, this);
    static_cast<FocusClearLineEdit*>(qEdit)->otherField = vEdit;
    calcButton = new QPushButton("Calculate", this);
    resultLabel = new QLabel("Enter m/z and one other value to calculate the third.", this);
    errorLabel = new QLabel("", this);
    errorLabel->setStyleSheet("color: red;");

    layout->addWidget(mzLabel, 0, 0);
    layout->addWidget(mzEdit, 0, 1);
    layout->addWidget(hrLine, 1, 0, 1, 2);
    layout->addWidget(qLabel, 2, 0);
    layout->addWidget(qEdit, 2, 1);
    layout->addWidget(vLabel, 3, 0);
    layout->addWidget(vEdit, 3, 1);
    layout->addWidget(calcButton, 4, 0, 1, 2);
    layout->addWidget(resultLabel, 5, 0, 1, 2);
    layout->addWidget(errorLabel, 6, 0, 1, 2);

    connect(calcButton, &QPushButton::clicked, this, &MiniCalculator::onInputChanged);

    connect(qEdit, &QLineEdit::selectionChanged, this, [this]() {
        if (qEdit->hasFocus()) {
            vEdit->clear();
        }
    });
    connect(vEdit, &QLineEdit::selectionChanged, this, [this]() {
        if (vEdit->hasFocus()) {
            qEdit->clear();
        }
    });
}

MiniCalculator::~MiniCalculator() {}

void MiniCalculator::onInputChanged() {
    errorLabel->clear();
    bool mzOk = false, qOk = false, vOk = false;
    double mz = mzEdit->text().toDouble(&mzOk);
    double q = qEdit->text().toDouble(&qOk);
    double v = vEdit->text().toDouble(&vOk);

    // Validation: m/z must be filled and valid
    if (mzEdit->text().isEmpty()) {
        errorLabel->setText("m/z must be filled in.");
        resultLabel->setText("");
        return;
    }
    if (!mzOk) {
        errorLabel->setText("m/z must be a valid number.");
        resultLabel->setText("");
        return;
    }
    // Of q and v, exactly one must be blank and the other valid
    bool qFilled = !qEdit->text().isEmpty();
    bool vFilled = !vEdit->text().isEmpty();
    if (qFilled == vFilled) {
        errorLabel->setText("Fill in either q or V trapping, but not both.");
        resultLabel->setText("");
        return;
    }
    if ((qFilled && !qOk) || (vFilled && !vOk)) {
        errorLabel->setText("Filled field must be a valid number.");
        resultLabel->setText("");
        return;
    }

    calculateMissingValue();
}

void MiniCalculator::setParameters(double r, double freq, int z) {
    radius = r;
    frequency = freq;
    chargeState = z;
}

void MiniCalculator::calculateMissingValue() {
    double mz = mzEdit->text().toDouble();
    double q = qEdit->text().toDouble();
    double v = vEdit->text().toDouble();
    double omega = mathieu_lib::omega(frequency);
    int z = chargeState;

    if (qEdit->text().isEmpty()) {
        // Calculate q from m/z and V trapping
        if (mz != 0 && radius != 0 && omega != 0) {
            double q_calc = ((4.0 * (v / 2) * E_CHARGE) /
                             ((mz / AVOGADRO_NUMBER) * (radius * radius) * (omega * omega)) * 1000);
            qEdit->setText(QString::number(q_calc));
            resultLabel->setText(QString("Calculated q."));
        } else {
            errorLabel->setText("Cannot calculate q: m/z, radius, or frequency is zero.");
            resultLabel->setText("");
        }
    } else if (vEdit->text().isEmpty()) {
        // Calculate V trapping from m/z and q
        if (z != 0) {
            double v_calc = ((mz / AVOGADRO_NUMBER) * 2.0 * q * radius * radius * omega * omega) /
                            (4.0 * z * E_CHARGE * 1000.0);
            vEdit->setText(QString::number(v_calc));
            resultLabel->setText(QString("Calculated V trapping."));
        } else {
            errorLabel->setText("Cannot calculate V trapping: charge state is zero.");
            resultLabel->setText("");
        }
    }
}
