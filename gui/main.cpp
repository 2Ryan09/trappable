#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

#include "MathieuWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/assets/bayspec_logo_no_words.ico"));
    try {
        trappable::MathieuWindow window;
        window.show();
        return app.exec();
    } catch (const std::exception &ex) {
        qCritical() << "Exception during app startup:" << ex.what();
        return 1;
    } catch (...) {
        qCritical() << "Unknown exception during app startup.";
        return 2;
    }
}
