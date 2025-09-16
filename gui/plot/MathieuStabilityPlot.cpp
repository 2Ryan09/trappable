#include "MathieuStabilityPlot.h"

#include <QVBoxLayout>

#include "QCustomPlot/qcustomplot.h"

MathieuStabilityPlot::MathieuStabilityPlot(QWidget *parent) : QWidget(parent) {
    customPlot = new QCustomPlot(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(customPlot);
    setLayout(layout);
    plotSkeleton();
}

MathieuStabilityPlot::~MathieuStabilityPlot() {
    // QCustomPlot will be deleted by Qt parent-child mechanism
}

void MathieuStabilityPlot::plotSkeleton() {
    // Skeleton: just axes and a placeholder curve
    QVector<double> a(2), q(2);
    a[0] = 0;
    a[1] = 1;
    q[0] = 0;
    q[1] = 1;
    customPlot->addGraph();
    customPlot->graph(0)->setData(a, q);
    customPlot->xAxis->setLabel("a");
    customPlot->yAxis->setLabel("q");
    customPlot->xAxis->setRange(-1, 2);
    customPlot->yAxis->setRange(-1, 2);
    customPlot->replot();
}
