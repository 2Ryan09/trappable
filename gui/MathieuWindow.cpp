

#include "MathieuWindow.h"

#include <QDoubleValidator>
#include <QFormLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "mathieu_lib/mathieu.h"

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
    auto* inputGroup = new QGroupBox(QStringLiteral("Inputs"), leftWidget);
    auto* formLayout = new QFormLayout(inputGroup);

    auto makeLineEdit = [this](const char* name) {
        auto* edit = new QLineEdit;
        edit->setObjectName(QString::fromUtf8(name));
        return edit;
    };
    frequencyEdit = makeLineEdit("frequencyEdit");
    radiusEdit = makeLineEdit("radiusEdit");
    massEdit = makeLineEdit("massEdit");
    voltageRfEdit = makeLineEdit("voltageRfEdit");
    voltageRfMaxEdit = makeLineEdit("voltageRfMaxEdit");
    voltageDcEdit = makeLineEdit("voltageDcEdit");
    chargeStateEdit = makeLineEdit("chargeStateEdit");

    frequencyEdit->setValidator(new QDoubleValidator(this));
    radiusEdit->setValidator(new QDoubleValidator(this));
    massEdit->setValidator(new QDoubleValidator(this));
    voltageRfEdit->setValidator(new QDoubleValidator(this));
    voltageRfMaxEdit->setValidator(new QDoubleValidator(this));
    voltageDcEdit->setValidator(new QDoubleValidator(this));
    chargeStateEdit->setValidator(new QIntValidator(this));

    formLayout->addRow(QStringLiteral("Frequency (Hz):"), frequencyEdit);
    formLayout->addRow(QStringLiteral("Quadrupole radius (m):"), radiusEdit);
    formLayout->addRow(QStringLiteral("Molar mass (kg/mol):"), massEdit);
    formLayout->addRow(QStringLiteral("RF Voltage (V):"), voltageRfEdit);
    formLayout->addRow(QStringLiteral("RF Voltage (max, V):"), voltageRfMaxEdit);
    formLayout->addRow(QStringLiteral("DC Voltage (V):"), voltageDcEdit);
    formLayout->addRow(QStringLiteral("Charge state:"), chargeStateEdit);
    inputGroup->setLayout(formLayout);
    leftLayout->addWidget(inputGroup);

    calcButton = new QPushButton(QStringLiteral("Calculate"));
    calcButton->setObjectName(QStringLiteral("calcButton"));
    calcButton->setEnabled(false);
    leftLayout->addWidget(calcButton);

    // Helper lambda to check all input validity
    auto validateInputs = [this]() { this->validateInputs(); };

    // Connect input edits to validation
    connect(frequencyEdit, &QLineEdit::textChanged, this, validateInputs);
    connect(radiusEdit, &QLineEdit::textChanged, this, validateInputs);
    connect(massEdit, &QLineEdit::textChanged, this, validateInputs);
    connect(voltageRfEdit, &QLineEdit::textChanged, this, validateInputs);
    connect(voltageRfMaxEdit, &QLineEdit::textChanged, this, validateInputs);
    connect(voltageDcEdit, &QLineEdit::textChanged, this, validateInputs);
    connect(chargeStateEdit, &QLineEdit::textChanged, this, validateInputs);

    // Add horizontal separator)
    auto* separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    leftLayout->addWidget(separator);

    auto* outputGroup = new QGroupBox(QStringLiteral("Outputs"), leftWidget);
    auto* outputLayout = new QGridLayout(outputGroup);
    int row = 0;
    omegaValueLabel = new QLabel(leftWidget);
    omegaUnitLabel = new QLabel(QStringLiteral("Hz"), leftWidget);
    outputLayout->addWidget(new QLabel(QStringLiteral("Omega:")), row, 0);
    outputLayout->addWidget(omegaValueLabel, row, 1);
    outputLayout->addWidget(omegaUnitLabel, row++, 2);

    particleMassValueLabel = new QLabel(leftWidget);
    particleMassUnitLabel = new QLabel(QStringLiteral("kg"), leftWidget);
    outputLayout->addWidget(new QLabel(QStringLiteral("Particle mass:")), row, 0);
    outputLayout->addWidget(particleMassValueLabel, row, 1);
    outputLayout->addWidget(particleMassUnitLabel, row++, 2);

    mathieuQValueLabel = new QLabel(leftWidget);
    mathieuQUnitLabel = new QLabel(QStringLiteral(""), leftWidget);
    outputLayout->addWidget(new QLabel(QStringLiteral("Mathieu q:")), row, 0);
    outputLayout->addWidget(mathieuQValueLabel, row, 1);
    outputLayout->addWidget(mathieuQUnitLabel, row++, 2);

    mathieuAValueLabel = new QLabel(leftWidget);
    mathieuAUnitLabel = new QLabel(QStringLiteral(""), leftWidget);
    outputLayout->addWidget(new QLabel(QStringLiteral("Mathieu a:")), row, 0);
    outputLayout->addWidget(mathieuAValueLabel, row, 1);
    outputLayout->addWidget(mathieuAUnitLabel, row++, 2);

    betaValueLabel = new QLabel(leftWidget);
    betaUnitLabel = new QLabel(QStringLiteral(""), leftWidget);
    outputLayout->addWidget(new QLabel(QStringLiteral("Beta:")), row, 0);
    outputLayout->addWidget(betaValueLabel, row, 1);
    outputLayout->addWidget(betaUnitLabel, row++, 2);

    secularFrequencyValueLabel = new QLabel(leftWidget);
    secularFrequencyUnitLabel = new QLabel(QStringLiteral("Hz"), leftWidget);
    outputLayout->addWidget(new QLabel(QStringLiteral("Secular frequency:")), row, 0);
    outputLayout->addWidget(secularFrequencyValueLabel, row, 1);
    outputLayout->addWidget(secularFrequencyUnitLabel, row++, 2);

    mzValueLabel = new QLabel(leftWidget);
    mzUnitLabel = new QLabel(QStringLiteral("Da"), leftWidget);
    outputLayout->addWidget(new QLabel(QStringLiteral("m/z:")), row, 0);
    outputLayout->addWidget(mzValueLabel, row, 1);
    outputLayout->addWidget(mzUnitLabel, row++, 2);

    lmcoValueLabel = new QLabel(leftWidget);
    lmcoUnitLabel = new QLabel(QStringLiteral("Da"), leftWidget);
    outputLayout->addWidget(new QLabel(QStringLiteral("LMCO:")), row, 0);
    outputLayout->addWidget(lmcoValueLabel, row, 1);
    outputLayout->addWidget(lmcoUnitLabel, row++, 2);

    maxMzValueLabel = new QLabel(leftWidget);
    maxMzUnitLabel = new QLabel(QStringLiteral("Da"), leftWidget);
    outputLayout->addWidget(new QLabel(QStringLiteral("Max m/z:")), row, 0);
    outputLayout->addWidget(maxMzValueLabel, row, 1);
    outputLayout->addWidget(maxMzUnitLabel, row++, 2);

    outputGroup->setLayout(outputLayout);
    leftLayout->addWidget(outputGroup);

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

    connect(
        calcButton, &QPushButton::clicked, this, [this]() { this->handleCalculation(); },
        Qt::QueuedConnection);
}

MathieuWindow::~MathieuWindow() {
    // All child widgets are deleted by Qt's parent-child mechanism
}

// --- Private methods for organization ---
/**
 * @brief Validate all input fields and enable/disable the Calculate button accordingly.
 *        Also sets tooltips for invalid fields.
 */
void MathieuWindow::validateInputs() {
    bool ok_freq = false, ok_radius = false, ok_mass = false, ok_voltage_rf = false,
         ok_voltage_rf_max = false, ok_voltage_dc = false, ok_charge_state = false;
    frequencyEdit->text().toDouble(&ok_freq);
    radiusEdit->text().toDouble(&ok_radius);
    massEdit->text().toDouble(&ok_mass);
    voltageRfEdit->text().toDouble(&ok_voltage_rf);
    voltageRfMaxEdit->text().toDouble(&ok_voltage_rf_max);
    voltageDcEdit->text().toDouble(&ok_voltage_dc);
    chargeStateEdit->text().toInt(&ok_charge_state);
    bool allValid = ok_freq && ok_radius && ok_mass && ok_voltage_rf && ok_voltage_rf_max &&
                    ok_voltage_dc && ok_charge_state;
    calcButton->setEnabled(allValid);
    frequencyEdit->setToolTip(ok_freq ? QStringLiteral("")
                                      : QStringLiteral("Enter a valid frequency (Hz)"));
    radiusEdit->setToolTip(ok_radius ? QStringLiteral("")
                                     : QStringLiteral("Enter a valid quadrupole radius (m)"));
    massEdit->setToolTip(ok_mass ? QStringLiteral("")
                                 : QStringLiteral("Enter a valid molar mass (kg/mol)"));
    voltageRfEdit->setToolTip(ok_voltage_rf ? QStringLiteral("")
                                            : QStringLiteral("Enter a valid RF voltage (V)"));
    voltageRfMaxEdit->setToolTip(ok_voltage_rf_max
                                     ? QStringLiteral("")
                                     : QStringLiteral("Enter a valid max RF voltage (V)"));
    voltageDcEdit->setToolTip(ok_voltage_dc ? QStringLiteral("")
                                            : QStringLiteral("Enter a valid DC voltage (V)"));
    chargeStateEdit->setToolTip(ok_charge_state
                                    ? QStringLiteral("")
                                    : QStringLiteral("Enter a valid charge state (integer)"));
}

/**
 * @brief Perform all scientific calculations and update output widgets and plot.
 *        Called when the Calculate button is pressed and inputs are valid.
 */
void MathieuWindow::handleCalculation() {
    bool ok_freq = false, ok_radius = false, ok_mass = false, ok_voltage_rf = false,
         ok_voltage_rf_max = false, ok_voltage_dc = false, ok_charge_state = false;
    double freq = frequencyEdit->text().toDouble(&ok_freq);
    double radius = radiusEdit->text().toDouble(&ok_radius);
    double mass = massEdit->text().toDouble(&ok_mass);
    double voltage_rf = voltageRfEdit->text().toDouble(&ok_voltage_rf);
    double voltage_rf_max = voltageRfMaxEdit->text().toDouble(&ok_voltage_rf_max);
    double voltage_dc = voltageDcEdit->text().toDouble(&ok_voltage_dc);
    int charge_state = chargeStateEdit->text().toInt(&ok_charge_state);
    if (!(ok_freq && ok_radius && ok_mass && ok_voltage_rf && ok_voltage_rf_max && ok_voltage_dc &&
          ok_charge_state)) {
        setOutputInvalid();
        return;
    }
    ::mathieu_lib::QuadrupoleParams params(freq, radius, mass);
    double omega_val = ::mathieu_lib::omega(freq);
    double particle_mass_val = ::mathieu_lib::particle_mass(mass);
    double mathieu_q_val = ::mathieu_lib::mathieu_q(voltage_rf, charge_state, params);
    double mathieu_a_val = ::mathieu_lib::mathieu_a(voltage_dc, charge_state, params);
    double beta_val = ::mathieu_lib::beta(mathieu_q_val);
    double secular_freq_val = ::mathieu_lib::secular_frequency(freq, mathieu_q_val);
    double mz_val = ::mathieu_lib::mz(voltage_rf, charge_state, params, mathieu_q_val);
    double lmco_val = ::mathieu_lib::lmco(voltage_rf, charge_state, params, ::mathieu_lib::MAX_Q);
    double max_mz_val =
        ::mathieu_lib::max_mz(voltage_rf_max, charge_state, params, ::mathieu_lib::MAX_Q);
    setOutputValues(omega_val, particle_mass_val, mathieu_q_val, mathieu_a_val, beta_val,
                    secular_freq_val, mz_val, lmco_val, max_mz_val);
    stabilityPlotter->plotPoint(mathieu_q_val, mathieu_a_val);
}

/**
 * @brief Set all output fields to "Invalid" when input validation fails.
 */
void MathieuWindow::setOutputInvalid() {
    omegaValueLabel->setText(QStringLiteral("Invalid"));
    particleMassValueLabel->setText(QStringLiteral("Invalid"));
    mathieuQValueLabel->setText(QStringLiteral("Invalid"));
    mathieuAValueLabel->setText(QStringLiteral("Invalid"));
    betaValueLabel->setText(QStringLiteral("Invalid"));
    secularFrequencyValueLabel->setText(QStringLiteral("Invalid"));
    mzValueLabel->setText(QStringLiteral("Invalid"));
    lmcoValueLabel->setText(QStringLiteral("Invalid"));
    maxMzValueLabel->setText(QStringLiteral("Invalid"));
}

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
void MathieuWindow::setOutputValues(double omega_val, double particle_mass_val,
                                    double mathieu_q_val, double mathieu_a_val, double beta_val,
                                    double secular_freq_val, double mz_val, double lmco_val,
                                    double max_mz_val) {
    auto formatValue = [](double val) {
        double absVal = std::abs(val);
        if ((absVal > 0 && (absVal < 0.001 || absVal >= 10000))) {
            return QString::number(val, 'e', 3);  // scientific notation, 3 decimals
        } else {
            return QString::number(val, 'f', 3);  // fixed, 3 decimals
        }
    };
    omegaValueLabel->setText(formatValue(omega_val));
    particleMassValueLabel->setText(formatValue(particle_mass_val));
    mathieuQValueLabel->setText(formatValue(mathieu_q_val));
    mathieuAValueLabel->setText(formatValue(mathieu_a_val));
    betaValueLabel->setText(formatValue(beta_val));
    secularFrequencyValueLabel->setText(formatValue(secular_freq_val));
    mzValueLabel->setText(formatValue(mz_val));
    lmcoValueLabel->setText(formatValue(lmco_val));
    maxMzValueLabel->setText(formatValue(max_mz_val));
}

}  // namespace trappable
