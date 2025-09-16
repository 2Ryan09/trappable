#include <QApplication>

#include "MathieuWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    trappable::MathieuWindow window;
    window.show();
    return app.exec();
}
