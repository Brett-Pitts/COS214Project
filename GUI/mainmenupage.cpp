#include "mainmenupage.h"
#include "ui_mainmenupage.h"

#include <QApplication>

MainMenuPage::MainMenuPage(QWidget *parent): QWidget(parent), ui(new Ui::MainMenuPage){
    ui->setupUi(this);
    connect(ui->manageCompanyButton, &QPushButton::clicked, this, &MainMenuPage::companyRequested);
    connect(ui->manageProjectsButton, &QPushButton::clicked, this, &MainMenuPage::projectRequested);
    connect(ui->assigntasksButton, &QPushButton::clicked, this, &MainMenuPage::taskRequested);
    connect(ui->exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
}

MainMenuPage::~MainMenuPage(){
    delete ui;
}
