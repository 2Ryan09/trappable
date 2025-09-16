#include <QApplication>

#include "MathieuWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MathieuWindow window;
    window.show();
    return app.exec();
}
