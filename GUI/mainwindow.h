#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

class ApplicationInterface;

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow :public QMainWindow {
	Q_OBJECT
public:
	MainWindow(int argc, char* argv[], QWidget* parent = NULL);
	~MainWindow() override;
private slots:
	void showMenu();
	void showCompany();
	void showProject();
	void showTasks();
	void showLoadSave();
	void workLoaded();
private:
	Ui::MainWindow* ui;
	ApplicationInterface* app;
};
#endif // MAINWINDOW_H
