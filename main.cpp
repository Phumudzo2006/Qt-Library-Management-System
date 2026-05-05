#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Library Management System");
    a.setOrganizationName("LibraryApp");
    MainWindow w;
    w.show();
    return a.exec();
}