#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow :public QMainWindow{
    Q_OBJECT
    public:
        MainWindow(QWidget* parent = NULL);
        ~MainWindow() override;
    private slots:
        void showMenu();
        void showCompany();
        void showProject();
        void showTasks();
    private:
        Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
