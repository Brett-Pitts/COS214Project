#ifndef MAINMENUPAGE_H
#define MAINMENUPAGE_H
#include <QWidget>

class ApplicationInterface;

namespace Ui{
    class MainMenuPage;
}

class MainMenuPage :public QWidget{
    Q_OBJECT
    public:
        MainMenuPage(QWidget* parent = NULL);
        ~MainMenuPage();
        void setApplication(ApplicationInterface* application);
        void refresh();
    signals:
        void companyRequested();
        void projectRequested();
        void taskRequested();
        void loadSaveRequested();
    private:
        Ui::MainMenuPage* ui;
        ApplicationInterface* app;
};

#endif // MAINMENUPAGE_H
