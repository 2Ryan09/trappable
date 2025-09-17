#include <QApplication>

#include "MathieuWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/assets/bayspec_logo_no_words.ico"));
    trappable::MathieuWindow window;
    window.show();
    return app.exec();
}
