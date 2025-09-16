#ifndef MATHIEUSTABILITYPLOT_H
#define MATHIEUSTABILITYPLOT_H

#include <QWidget>
class QCustomPlot;

class MathieuStabilityPlot : public QWidget {
    Q_OBJECT
   public:
    explicit MathieuStabilityPlot(QWidget *parent = nullptr);
    ~MathieuStabilityPlot();

    void plotSkeleton();  // For initial skeleton plot

   private:
    QCustomPlot *customPlot;
};

#endif  // MATHIEUSTABILITYPLOT_H
