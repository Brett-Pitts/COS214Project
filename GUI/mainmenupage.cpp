#include "mainmenupage.h"
#include "ui_mainmenupage.h"

#include <QApplication>
#include <QLabel>

#include "ApplicationInterface.h"

namespace {

void showStatus(QLabel* label, const QString& name, bool loaded){
    if(loaded){
        label->setText(name + " loaded");
        label->setStyleSheet("color: #1a7f37; font-weight: bold;");
    }
    else{
        label->setText(name + " not loaded");
        label->setStyleSheet("color: #b42318; font-weight: bold;");
    }
}

}

MainMenuPage::MainMenuPage(QWidget *parent): QWidget(parent), ui(new Ui::MainMenuPage), app(nullptr){
    ui->setupUi(this);
    connect(ui->manageCompanyButton, &QPushButton::clicked, this, &MainMenuPage::companyRequested);
    connect(ui->manageProjectsButton, &QPushButton::clicked, this, &MainMenuPage::projectRequested);
    connect(ui->assigntasksButton, &QPushButton::clicked, this, &MainMenuPage::taskRequested);
    connect(ui->loadSaveButton, &QPushButton::clicked, this, &MainMenuPage::loadSaveRequested);
    connect(ui->exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    showStatus(ui->companyStatusLabel, "Company", false);
    showStatus(ui->projectStatusLabel, "Project", false);
}

MainMenuPage::~MainMenuPage(){
    delete ui;
}

void MainMenuPage::setApplication(ApplicationInterface* application){
    app = application;
    refresh();
}

void MainMenuPage::refresh(){
    if(app == nullptr) return;
    showStatus(ui->companyStatusLabel, "Company", app->isLoaded(MementoType::company));
    showStatus(ui->projectStatusLabel, "Project", app->isLoaded(MementoType::workflow));
}
