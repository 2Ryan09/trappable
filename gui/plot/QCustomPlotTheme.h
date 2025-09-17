#pragma once
#include <QCustomPlot/qcustomplot.h>

#include <QApplication>
#include <QPalette>

namespace QCustomPlotTheme {
inline void applyLightTheme(QCustomPlot* plot) {
    plot->axisRect()->setBackground(Qt::white);
    plot->xAxis->setLabelColor(Qt::black);
    plot->yAxis->setLabelColor(Qt::black);
    plot->xAxis->setTickLabelColor(Qt::black);
    plot->yAxis->setTickLabelColor(Qt::black);
    plot->xAxis->grid()->setPen(QPen(QColor(220, 220, 220)));
    plot->yAxis->grid()->setPen(QPen(QColor(220, 220, 220)));
    plot->xAxis->setBasePen(QPen(Qt::black));
    plot->yAxis->setBasePen(QPen(Qt::black));
    plot->xAxis->setTickPen(QPen(Qt::black));
    plot->yAxis->setTickPen(QPen(Qt::black));
    plot->xAxis->setSubTickPen(QPen(Qt::black));
    plot->yAxis->setSubTickPen(QPen(Qt::black));
}

inline void applyDarkTheme(QCustomPlot* plot) {
    plot->axisRect()->setBackground(QColor(30, 30, 30));
    plot->xAxis->setLabelColor(Qt::white);
    plot->yAxis->setLabelColor(Qt::white);
    plot->xAxis->setTickLabelColor(Qt::white);
    plot->yAxis->setTickLabelColor(Qt::white);
    plot->xAxis->grid()->setPen(QPen(QColor(60, 60, 60)));
    plot->yAxis->grid()->setPen(QPen(QColor(60, 60, 60)));
    plot->xAxis->setBasePen(QPen(Qt::white));
    plot->yAxis->setBasePen(QPen(Qt::white));
    plot->xAxis->setTickPen(QPen(Qt::white));
    plot->yAxis->setTickPen(QPen(Qt::white));
    plot->xAxis->setSubTickPen(QPen(Qt::white));
    plot->yAxis->setSubTickPen(QPen(Qt::white));
}

inline void applySystemTheme(QCustomPlot* plot) {
    QPalette palette = QApplication::palette();
    if (palette.color(QPalette::Window).lightness() < 128) {
        applyDarkTheme(plot);
    } else {
        applyLightTheme(plot);
    }
}
}  // namespace QCustomPlotTheme
