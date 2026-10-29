#ifndef MAINMENUPAGE_H
#define MAINMENUPAGE_H
#include <QWidget>

namespace Ui{
    class MainMenuPage;
}

class MainMenuPage :public QWidget{
    Q_OBJECT
    public:
        MainMenuPage(QWidget* parent = NULL);
        ~MainMenuPage();
    signals:
        void companyRequested();
        void projectRequested();
    private:
        Ui::MainMenuPage* ui;
};

#endif // MAINMENUPAGE_H
