#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
#ifdef _WIN32
    a.setStyleSheet("QPushButton, QLabel, QLineEdit{font-family: Microsoft YaHei}");
#endif
    MainWindow w;
    w.show();
    return QApplication::exec();
}
