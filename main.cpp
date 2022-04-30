#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QLabel, QLineEdit, QPushButton {font-family: \"Microsoft YaHei\"}");
    MainWindow w;
    w.show();
    return QApplication::exec();
}
