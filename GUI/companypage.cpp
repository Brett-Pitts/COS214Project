#include "companypage.h"
#include "ui_companypage.h"

CompanyPage::CompanyPage(QWidget *parent): QWidget(parent), ui(new Ui::CompanyPage){
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &CompanyPage::backRequested);
}

CompanyPage::~CompanyPage(){
    delete ui;
}
