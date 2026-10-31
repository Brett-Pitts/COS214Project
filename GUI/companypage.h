#ifndef COMPANYPAGE_H
#define COMPANYPAGE_H
#include <QWidget>
#include <QHash>

class ApplicationInterface;
class CompanyNode;
class Team;
class QTreeWidgetItem;

namespace Ui{
    class CompanyPage;
}

class CompanyPage :public QWidget{
    Q_OBJECT
    public:
        CompanyPage(QWidget* parent = NULL);
        ~CompanyPage();
        void setApplication(ApplicationInterface* application);
        void refresh();
    signals:
        void backRequested();
    private slots:
        void onTreeSelectionChanged();
        void onSubItemSelectionChanged();
        void onEmployeeSelectionChanged();
        void addDepartment();
        void addTeam();
        void renameSelected();
        void deleteSelected();
        void openSubItem();
        void addSubDepartment();
        void addSubTeam();
        void addEmployee();
        void removeEmployee();
        void changeCompetency();
    private:
        CompanyNode* selectedNode() const;
        CompanyNode* shownNode() const;
        Team* shownTeam() const;
        void addChildItems(QTreeWidgetItem* parentItem, CompanyNode* node);
        void showDepartment(CompanyNode* node, bool isRoot);
        void showTeam(Team* team);
        void createDepartmentIn(CompanyNode* parent);
        void createTeamIn(CompanyNode* parent);
        CompanyNode* containerFor(CompanyNode* node) const;
        void selectNode(CompanyNode* node);

        Ui::CompanyPage* ui;
        ApplicationInterface* app;
        QHash<CompanyNode*, QTreeWidgetItem*> items;
};

#endif // COMPANYPAGE_H
