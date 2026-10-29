#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
  ui->stackedWidget->setCurrentWidget(ui->menuPage);
    //how each workj
   // when clicked fires, backRequested is emitted automatically (no emit needed); connect takes 4 things: sender object, its signal, receiver object, and the slot or signal to run
    connect(ui->menuPage, &MainMenuPage::companyRequested, this, &MainWindow::showCompany); //each to go to their requested page by button press
    connect(ui->menuPage, &MainMenuPage::projectRequested, this, &MainWindow::showProject);
    connect(ui->companyPage, &CompanyPage::backRequested, this, &MainWindow::showMenu); // to go back to menu
    connect(ui->projectPage, &ProjectPage::backRequested, this, &MainWindow::showMenu);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::showMenu(){
    ui->stackedWidget->setCurrentWidget(ui->menuPage); //only show the menu page widget
}

void MainWindow::showCompany(){
    ui->stackedWidget->setCurrentWidget(ui->companyPage);
}

void MainWindow::showProject(){
    ui->stackedWidget->setCurrentWidget(ui->projectPage);
}
