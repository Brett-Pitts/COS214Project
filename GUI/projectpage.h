#ifndef PROJECTPAGE_H
#define PROJECTPAGE_H
#include <QWidget>

namespace Ui{
    class ProjectPage;
}

class ProjectPage :public QWidget{
    Q_OBJECT
    public:
        ProjectPage(QWidget* parent = NULL);
        ~ProjectPage();
    signals:
        void backRequested();
    private:
        Ui::ProjectPage* ui;
};

#endif // PROJECTPAGE_H
