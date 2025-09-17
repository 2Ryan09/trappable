
#include "MathieuWindow.h"

#include <QDoubleValidator>
#include <QFormLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "Inputs.h"
#include "Outputs.h"
#include "mathieu_lib/mathieu.h"
#include "stability/StabilityCalculator.h"
#include "stability/StabilityOutputs.h"

/**
 * @class MathieuWindow
 * @brief Main window for the Mathieu quadrupole stability calculator GUI.
 *
 * Provides input fields, output display, and stability region plotting for scientific analysis
 * of quadrupole mass filters using the Mathieu equations. All scientific calculations are
 * delegated to mathieu_lib. UI is modular and follows Qt best practices.
 */
namespace trappable {

constexpr double MAX_Q = 0.908;

/**
 * @brief Construct the main window and initialize all widgets, layouts, and connections.
 * @param parent Optional parent widget.
 */
MathieuWindow::MathieuWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle(QStringLiteral("Mathieu Quadrupole Stability Calculator"));
    // Use a horizontal layout: left = inputs/results, right = plot
    auto* mainLayout = new QHBoxLayout(this);
    // Left side: inputs and outputs
    auto* leftWidget = new QWidget(this);
    auto* leftLayout = new QVBoxLayout(leftWidget);
    inputs = new Inputs(leftWidget);
    leftLayout->addWidget(inputs);

    calcButton = new QPushButton(QStringLiteral("Calculate"));
    calcButton->setObjectName(QStringLiteral("calcButton"));
    calcButton->setEnabled(false);
    leftLayout->addWidget(calcButton);

    auto* separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    leftLayout->addWidget(separator);

    outputs = new Outputs(leftWidget);
    leftLayout->addWidget(outputs);

    leftLayout->addStretch();
    mainLayout->addWidget(leftWidget, 0);  // left side, stretch factor 0
    leftLayout->addStretch();
    mainLayout->addWidget(leftWidget, 0);  // left side, stretch factor 0

    // Right side: large stability plot
    stabilityPlotWidget = new QCustomPlot(this);
    stabilityPlotWidget->setMinimumWidth(600);
    stabilityPlotWidget->setMinimumHeight(400);
    stabilityPlotWidget->setBackground(Qt::transparent);
    // Add axes on the right and top
    QCPAxis* rightAxis = stabilityPlotWidget->axisRect()->addAxis(QCPAxis::atRight);
    QCPAxis* topAxis = stabilityPlotWidget->axisRect()->addAxis(QCPAxis::atTop);
    rightAxis->setLabel("");
    topAxis->setLabel("");
    rightAxis->setTickLabels(false);
    topAxis->setTickLabels(false);
    mainLayout->addWidget(stabilityPlotWidget, 1);  // right side, stretch factor 1

    stabilityPlotter = new StabilityRegionPlotter(stabilityPlotWidget);

    // Add plot labels for stability regions
    QCPItemText* stableLabel = new QCPItemText(stabilityPlotWidget);
    stableLabel->setPositionAlignment(Qt::AlignCenter);
    stableLabel->position->setType(QCPItemPosition::ptPlotCoords);
    stableLabel->position->setCoords(0.65, 0.15);
    stableLabel->setText("stable");
    stableLabel->setFont(QFont(font().family(), 12, QFont::Bold));
    stableLabel->setColor(Qt::darkGreen);

    QCPItemText* unstableLabel = new QCPItemText(stabilityPlotWidget);
    unstableLabel->setPositionAlignment(Qt::AlignCenter);
    unstableLabel->position->setType(QCPItemPosition::ptPlotCoords);
    unstableLabel->position->setCoords(0.1, 0.15);
    unstableLabel->setText("unstable");
    unstableLabel->setFont(QFont(font().family(), 12, QFont::Bold));
    unstableLabel->setColor(Qt::red);

    // Add StabilityOutputs component below the plot
    stabilityOutputs = new StabilityOutputs(this);
    auto* rightLayout = new QVBoxLayout;
    rightLayout->addWidget(stabilityPlotWidget);
    rightLayout->addWidget(stabilityOutputs);
    auto* rightWidget = new QWidget(this);
    rightWidget->setLayout(rightLayout);
    mainLayout->addWidget(rightWidget, 1);

    connect(
        calcButton, &QPushButton::clicked, this, [this]() { this->handleCalculation(); },
        Qt::QueuedConnection);

    // Connect all input fields to validation logic
    auto connectInputValidation = [this](QLineEdit* edit) {
        connect(edit, &QLineEdit::textChanged, this, [this]() { this->validateInputs(); });
    };
    connectInputValidation(inputs->frequencyEdit);
    connectInputValidation(inputs->radiusEdit);
    connectInputValidation(inputs->massEdit);
    connectInputValidation(inputs->voltageRfEdit);
    connectInputValidation(inputs->voltageRfMaxEdit);
    connectInputValidation(inputs->voltageDcEdit);
    connectInputValidation(inputs->chargeStateEdit);

    // Also connect unit radio buttons to validation
    auto connectUnitValidation = [this](QRadioButton* radio) {
        connect(radio, &QRadioButton::toggled, this, [this]() { this->validateInputs(); });
    };
    connectUnitValidation(inputs->frequencyUnitHz);
    connectUnitValidation(inputs->frequencyUnitKHz);
    connectUnitValidation(inputs->radiusUnitM);
    connectUnitValidation(inputs->radiusUnitMM);
    connectUnitValidation(inputs->massUnitKg);
    connectUnitValidation(inputs->massUnitG);
    connectUnitValidation(inputs->voltageRfUnitV);
    connectUnitValidation(inputs->voltageRfUnitMV);
    connectUnitValidation(inputs->voltageRfMaxUnitV);
    connectUnitValidation(inputs->voltageRfMaxUnitMV);
    connectUnitValidation(inputs->voltageDcUnitV);
    connectUnitValidation(inputs->voltageDcUnitMV);
    // Ensure initial validation state
    validateInputs();
}

MathieuWindow::~MathieuWindow() {
    // All child widgets are deleted by Qt's parent-child mechanism
}

// --- Private methods for organization ---
/**
 * @brief Validate all input fields and enable/disable the Calculate button accordingly.
 *        Also sets tooltips for invalid fields.
 */
// Input validation is now handled by Inputs component
void MathieuWindow::validateInputs() {
    bool allValid = inputs->validate();
    calcButton->setEnabled(allValid);
}

/**
 * @brief Perform all scientific calculations and update output widgets and plot.
 *        Called when the Calculate button is pressed and inputs are valid.
 */
void MathieuWindow::handleCalculation() {
    Inputs::CalculationInputs calcInputs;
    bool ok = inputs->getCalculationInputs(calcInputs);
    if (!ok) {
        outputs->setInvalid();
        return;
    }
    ::mathieu_lib::QuadrupoleParams params(calcInputs.freq, calcInputs.radius, calcInputs.mass);
    double omega_val = ::mathieu_lib::omega(calcInputs.freq);
    double particle_mass_val = ::mathieu_lib::particle_mass(calcInputs.mass);
    double mathieu_q_val =
        ::mathieu_lib::mathieu_q(calcInputs.voltage_rf, calcInputs.charge_state, params);
    double mathieu_a_val =
        ::mathieu_lib::mathieu_a(calcInputs.voltage_dc, calcInputs.charge_state, params);
    double beta_val = ::mathieu_lib::beta(mathieu_q_val);
    double secular_freq_val = ::mathieu_lib::secular_frequency(calcInputs.freq, mathieu_q_val);
    double mz_val =
        ::mathieu_lib::mz(calcInputs.voltage_rf, calcInputs.charge_state, params, mathieu_q_val);
    double lmco_val = ::mathieu_lib::lmco(calcInputs.voltage_rf, calcInputs.charge_state, params,
                                          ::mathieu_lib::MAX_Q);
    double max_mz_val = ::mathieu_lib::max_mz(calcInputs.voltage_rf_max, calcInputs.charge_state,
                                              params, ::mathieu_lib::MAX_Q);
    outputs->setValues(omega_val, particle_mass_val, mathieu_q_val, mathieu_a_val, beta_val,
                       secular_freq_val, mz_val, lmco_val, max_mz_val);
    stabilityPlotter->plotPoint(mathieu_q_val, mathieu_a_val);

    // Calculate and update stability outputs using nearest boundary point
    auto [q_b, a_b] = StabilityCalculator::findNearestBoundaryPoint(mathieu_q_val, mathieu_a_val);
    double delta_a = a_b - mathieu_a_val;
    double delta_q = q_b - mathieu_q_val;
    double delta_e = qSqrt((mathieu_q_val - q_b) * (mathieu_q_val - q_b) +
                           (mathieu_a_val - a_b) * (mathieu_a_val - a_b));
    double theta = StabilityCalculator::angularOffset(mathieu_a_val, a_b, mathieu_q_val, q_b);
    double s_norm = delta_e / qSqrt(a_b * a_b + q_b * q_b);
    double delta_min = delta_e;

    constexpr double e_charge = 1.602176634e-19;
    double voltage_diff =
        delta_a *
        (particle_mass_val * calcInputs.radius * calcInputs.radius * omega_val * omega_val) /
        (2.0 * e_charge * calcInputs.charge_state);
    double resolution = (mathieu_a_val != 0.0) ? delta_a / std::abs(mathieu_a_val) : 0.0;

    stabilityOutputs->setDeltaA(delta_a);
    stabilityOutputs->setDeltaQ(delta_q);
    stabilityOutputs->setDeltaE(delta_e);
    stabilityOutputs->setTheta(theta);
    stabilityOutputs->setSNorm(s_norm);
    stabilityOutputs->setDeltaMin(delta_min);
    stabilityOutputs->setVoltageDiff(voltage_diff);
    stabilityOutputs->setResolution(resolution);

    // Draw right triangle indicators
    stabilityPlotter->drawNearestPointTriangle(mathieu_q_val, mathieu_a_val, q_b, a_b);
}

/**
 * @brief Set all output fields to "Invalid" when input validation fails.
 */
// Output invalidation is now handled by Outputs component
void MathieuWindow::setOutputInvalid() { outputs->setInvalid(); }
// removed stray brace

/**
 * @brief Set all output fields to the calculated values, formatted for scientific readability.
 * @param omega_val Angular frequency (Hz)
 * @param particle_mass_val Particle mass (kg)
 * @param mathieu_q_val Mathieu q parameter
 * @param mathieu_a_val Mathieu a parameter
 * @param beta_val Beta parameter
 * @param secular_freq_val Secular frequency (Hz)
 * @param mz_val m/z value (Da)
 * @param lmco_val LMCO value (Da)
 * @param max_mz_val Maximum m/z value (Da)
 */
// Output value setting is now handled by Outputs component
void MathieuWindow::setOutputValues(double omega_val, double particle_mass_val,
                                    double mathieu_q_val, double mathieu_a_val, double beta_val,
                                    double secular_freq_val, double mz_val, double lmco_val,
                                    double max_mz_val) {
    outputs->setValues(omega_val, particle_mass_val, mathieu_q_val, mathieu_a_val, beta_val,
                       secular_freq_val, mz_val, lmco_val, max_mz_val);
    // removed stray brace
}

}  // namespace trappable
