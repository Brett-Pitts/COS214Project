
#include "taskpage.h"
#include "ui_taskpage.h"

TaskPage::TaskPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::TaskPage)
{
    ui->setupUi(this);
    ui->assignmentTable->setColumnWidth(0, 220);

    connect(ui->backButton, &QPushButton::clicked,
            this, &TaskPage::backRequested);

    connect(ui->workUnitList, &QListWidget::itemSelectionChanged,
            this, &TaskPage::updateSelection);
    connect(ui->assignmentTable, &QTableWidget::itemSelectionChanged,
            this, &TaskPage::updateSelection);

    updateSelection();
}

TaskPage::~TaskPage()
{
    delete ui;
}

void TaskPage::updateSelection()
{
    QList<QListWidgetItem *> picked = ui->workUnitList->selectedItems();
    bool unitPicked = !picked.isEmpty();

    ui->selectedWorkUnitLabel->setText(unitPicked ? picked.first()->text()
                                                  : "None selected");

    ui->assignButton->setEnabled(unitPicked);
    ui->unassignButton->setEnabled(!ui->assignmentTable->selectedItems().isEmpty());
}
