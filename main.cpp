#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    a.setStyleSheet("QLabel, QLineEdit, QPushButton {font-family: \"Microsoft YaHei\"}");
    MainWindow w;
    w.show();
    return QApplication::exec();
}
