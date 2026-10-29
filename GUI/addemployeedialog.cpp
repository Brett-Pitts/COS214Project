#include "addemployeedialog.h"
#include "ui_addemployeedialog.h"

AddEmployeeDialog::AddEmployeeDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddEmployeeDialog){
    ui->setupUi(this);
}

AddEmployeeDialog::~AddEmployeeDialog(){
    delete ui;
}
