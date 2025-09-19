#ifndef MINICALCULATOR_H
#define MINICALCULATOR_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class MiniCalculator : public QWidget {
    class FocusClearLineEdit : public QLineEdit {
       public:
        FocusClearLineEdit(QLineEdit* other, QWidget* parent = nullptr)
            : QLineEdit(parent), otherField(other) {}
        QLineEdit* otherField;

       protected:
        void focusInEvent(QFocusEvent* event) override {
            if (otherField) {
                otherField->clear();
            }
            QLineEdit::focusInEvent(event);
        }
    };
    Q_OBJECT
   public:
    explicit MiniCalculator(QWidget* parent = nullptr);
    ~MiniCalculator();
    void setParameters(double r, double freq, int z);
    void calculateMissingValue();
    double radius;            // no default
    double frequency;         // no default
    int chargeState;          // no default
    QPushButton* calcButton;  // Make public for external connection

   private slots:
    void onInputChanged();

   private:
    QLineEdit* mzEdit;
    QLineEdit* qEdit;
    QLineEdit* vEdit;
    QLabel* mzLabel;
    QLabel* qLabel;
    QLabel* vLabel;
    QLabel* resultLabel;
    QLabel* errorLabel;
    QFrame* hrLine;
};

#endif  // MINICALCULATOR_H
