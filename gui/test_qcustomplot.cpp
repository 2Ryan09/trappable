#include <QApplication>

#include "plot/QCustomPlot/qcustomplot.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCustomPlot plot;
    plot.show();
    return app.exec();
}
