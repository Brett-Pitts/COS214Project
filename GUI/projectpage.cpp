#include "projectpage.h"
#include "ui_projectpage.h"

ProjectPage::ProjectPage(QWidget *parent): QWidget(parent), ui(new Ui::ProjectPage){
    ui->setupUi(this);
    ui->projectTree->setColumnWidth(0, 200);
    connect(ui->backButton, &QPushButton::clicked, this, &ProjectPage::backRequested);
}

ProjectPage::~ProjectPage(){
    delete ui;
}
