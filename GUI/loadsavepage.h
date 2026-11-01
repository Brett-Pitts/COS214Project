#ifndef LOADSAVEPAGE_H
#define LOADSAVEPAGE_H
#include <QWidget>
#include <QString>
#include "MementoType.h"

class ApplicationInterface;

namespace Ui{
    class LoadSavePage;
}

class LoadSavePage :public QWidget{
    Q_OBJECT
    public:
        LoadSavePage(QWidget* parent = NULL);
        ~LoadSavePage();
        void setApplication(ApplicationInterface* application);
    signals:
        void backRequested();
        void loaded();
    private slots:
        void saveCompany();
        void loadCompany();
        void saveProjects();
        void loadProjects();
    private:
        void saveFile(MementoType type, const QString& title, const QString& fileName);
        void loadFile(MementoType type, const QString& title);

        Ui::LoadSavePage* ui;
        ApplicationInterface* app;
};

#endif // LOADSAVEPAGE_H
