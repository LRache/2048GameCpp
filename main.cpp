#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");
#ifdef _WIN32
    a.setStyleSheet("QPushButton, QLabel, QLineEdit{font-family: Microsoft YaHei}");
#endif
    MainWindow w;
    w.show();
    return QApplication::exec();
}
