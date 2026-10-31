#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]){QApplication a(argc, argv);
    QFile styleFile(":/style.qss");
    if(styleFile.open(QFile::ReadOnly)){
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }
    MainWindow w;
    w.show();
    return a.exec();
}
