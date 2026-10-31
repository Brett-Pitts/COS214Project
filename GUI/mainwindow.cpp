
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ApplicationInterface.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), app(new ApplicationInterface)
{
    ui->setupUi(this);

    ui->companyPage->setApplication(app);
    ui->projectPage->setApplication(app);
    ui->taskPage->setApplication(app);
    ui->menuPage->setApplication(app);
    ui->loadSavePage->setApplication(app);

    ui->stackedWidget->setCurrentWidget(ui->menuPage);

    connect(ui->menuPage, &MainMenuPage::companyRequested,
            this, &MainWindow::showCompany);

    connect(ui->menuPage, &MainMenuPage::projectRequested,
            this, &MainWindow::showProject);

    connect(ui->menuPage, &MainMenuPage::taskRequested,
            this, &MainWindow::showTasks);

    connect(ui->menuPage, &MainMenuPage::loadSaveRequested,
            this, &MainWindow::showLoadSave);

    connect(ui->loadSavePage, &LoadSavePage::backRequested,
            this, &MainWindow::showMenu);

    connect(ui->loadSavePage, &LoadSavePage::loaded,
            this, &MainWindow::workLoaded);

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
    delete app;
}

void MainWindow::showMenu()
{
    ui->menuPage->refresh();
    ui->stackedWidget->setCurrentWidget(ui->menuPage);
}

void MainWindow::showCompany()
{
    ui->companyPage->refresh();
    ui->stackedWidget->setCurrentWidget(ui->companyPage);
}

void MainWindow::showProject()
{
    ui->projectPage->refresh();
    ui->stackedWidget->setCurrentWidget(ui->projectPage);
}

void MainWindow::showTasks()
{
    ui->taskPage->refresh();
    ui->stackedWidget->setCurrentWidget(ui->taskPage);
}

void MainWindow::showLoadSave()
{
    ui->stackedWidget->setCurrentWidget(ui->loadSavePage);
}

void MainWindow::workLoaded()
{
    ui->taskPage->reset();
}
