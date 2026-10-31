#ifndef PROJECTPAGE_H
#define PROJECTPAGE_H
#include <QWidget>
#include <QHash>

class ApplicationInterface;
class WorkNode;
class QTreeWidgetItem;

namespace Ui{
    class ProjectPage;
}

class ProjectPage :public QWidget{
    Q_OBJECT
    public:
        ProjectPage(QWidget* parent = NULL);
        ~ProjectPage();
        void setApplication(ApplicationInterface* application);
        void refresh();
    signals:
        void backRequested();
    private slots:
        void onSelectionChanged();
        void addProject();
        void addSubProject();
        void addWorkUnit();
        void deleteNode();
        void saveChanges();
        void addPrerequisite();
        void removePrerequisite();
    private:
        WorkNode* selectedNode() const;
        void addChildItems(QTreeWidgetItem* parentItem, WorkNode* node);
        void selectNode(WorkNode* node);
        void fillCombos();

        Ui::ProjectPage* ui;
        ApplicationInterface* app;
        QHash<WorkNode*, QTreeWidgetItem*> items;
};

#endif // PROJECTPAGE_H
