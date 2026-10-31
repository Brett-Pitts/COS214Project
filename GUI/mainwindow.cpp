
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->menuPage);

    connect(ui->menuPage, &MainMenuPage::companyRequested,
            this, &MainWindow::showCompany);

    connect(ui->menuPage, &MainMenuPage::projectRequested,
            this, &MainWindow::showProject);

    connect(ui->menuPage, &MainMenuPage::taskRequested,
            this, &MainWindow::showTasks);

    connect(ui->companyPage, &CompanyPage::backRequested,
            this, &MainWindow::showMenu);

    connect(ui->projectPage, &ProjectPage::backRequested,
            this, &MainWindow::showMenu);

    connect(ui->taskPage, &TaskPage::backRequested,
            this, &MainWindow::showMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMenu()
{
    ui->stackedWidget->setCurrentWidget(ui->menuPage);
}

void MainWindow::showCompany()
{
    ui->stackedWidget->setCurrentWidget(ui->companyPage);
}

void MainWindow::showProject()
{
    ui->stackedWidget->setCurrentWidget(ui->projectPage);
}

void MainWindow::showTasks()
{
    ui->stackedWidget->setCurrentWidget(ui->taskPage);
}
