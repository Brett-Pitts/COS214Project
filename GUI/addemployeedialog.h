#ifndef ADDEMPLOYEEDIALOG_H
#define ADDEMPLOYEEDIALOG_H
#include <QDialog>

namespace Ui{
    class AddEmployeeDialog;
}

class AddEmployeeDialog :public QDialog{
    Q_OBJECT
    public:
        AddEmployeeDialog(QWidget* parent = NULL);
        ~AddEmployeeDialog();
    private:
        Ui::AddEmployeeDialog* ui;
};

#endif // ADDEMPLOYEEDIALOG_H
