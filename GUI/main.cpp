#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <iostream>

int main(int argc, char* argv[]) {
	std::cout << "Test\n";
	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
	QApplication a(argc, argv);
	QFile styleFile(":/style.qss");
	if (styleFile.open(QFile::ReadOnly)) {
		a.setStyleSheet(styleFile.readAll());
		styleFile.close();
	}
	MainWindow w(argc, argv);
	w.show();
	return a.exec();
}
