#include "loadsavepage.h"
#include "ui_loadsavepage.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

#include "ApplicationInterface.h"

namespace {

QString startPath(const QString& fileName){
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + fileName;
}

}

LoadSavePage::LoadSavePage(QWidget *parent): QWidget(parent), ui(new Ui::LoadSavePage), app(nullptr){
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &LoadSavePage::backRequested);
    connect(ui->saveCompanyButton, &QPushButton::clicked, this, &LoadSavePage::saveCompany);
    connect(ui->loadCompanyButton, &QPushButton::clicked, this, &LoadSavePage::loadCompany);
    connect(ui->saveProjectsButton, &QPushButton::clicked, this, &LoadSavePage::saveProjects);
    connect(ui->loadProjectsButton, &QPushButton::clicked, this, &LoadSavePage::loadProjects);
}

LoadSavePage::~LoadSavePage(){
    delete ui;
}

void LoadSavePage::setApplication(ApplicationInterface* application){
    app = application;
}

void LoadSavePage::saveCompany(){
    saveFile(MementoType::company, "company", "company.txt");
}

void LoadSavePage::loadCompany(){
    loadFile(MementoType::company, "company");
}

void LoadSavePage::saveProjects(){
    saveFile(MementoType::workflow, "projects", "projects.txt");
}

void LoadSavePage::loadProjects(){
    loadFile(MementoType::workflow, "projects");
}

void LoadSavePage::saveFile(MementoType type, const QString& title, const QString& fileName){
    if(app == nullptr) return;
    QString path = QFileDialog::getSaveFileName(this, "Save " + title, startPath(fileName), "TIVIDY files (*.txt)");
    if(path.isEmpty()) return;
    if(app->saveToFile(type, path.toStdString())){
        QMessageBox::information(this, "Save " + title, "Saved.");
    }
    else{
        QMessageBox::warning(this, "Save " + title, "That could not be saved to that location.");
    }
}

void LoadSavePage::loadFile(MementoType type, const QString& title){
    if(app == nullptr) return;
    QString path = QFileDialog::getOpenFileName(this, "Load " + title, startPath(""), "TIVIDY files (*.txt)");
    if(path.isEmpty()) return;
    if(app->loadFromFile(type, path.toStdString())){
        emit loaded();
        QMessageBox::information(this, "Load " + title, "Loaded.");
    }
    else{
        QMessageBox::warning(this, "Load " + title, "That file could not be loaded. Is it a " + title + " file saved by TIVIDY?");
    }
}
