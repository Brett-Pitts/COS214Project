
#ifndef TASKPAGE_H
#define TASKPAGE_H

#include <QWidget>
#include <vector>

class ApplicationInterface;
class WorkUnit;
class Team;

QT_BEGIN_NAMESPACE
namespace Ui {
class TaskPage;
}
QT_END_NAMESPACE

class TaskPage : public QWidget
{
    Q_OBJECT

public:
    explicit TaskPage(QWidget *parent = nullptr);
    ~TaskPage();

    void setApplication(ApplicationInterface *application);
    void refresh();
    void reset();

signals:
    void backRequested();

private slots:
    void updateSelection();
    void assign();
    void unassign();
    void stepSelected();
    void cancelSelected();
    void forceFailSelected();
    void forceStateSelected();

private:
    WorkUnit *selectedUnit() const;

    Ui::TaskPage *ui;
    ApplicationInterface *app;
    std::vector<WorkUnit *> units;
    std::vector<Team *> teamChoices;
    WorkUnit *teamsFor;
};

#endif
