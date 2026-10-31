
#ifndef TASKPAGE_H
#define TASKPAGE_H

#include <QWidget>

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

signals:
    void backRequested();

private slots:
    void updateSelection();

private:
    Ui::TaskPage *ui;
};

#endif
