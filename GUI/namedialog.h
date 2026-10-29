#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H
#include <QDialog>

namespace Ui{
    class NameDialog;
}

class NameDialog :public QDialog{
    Q_OBJECT
    public:
        NameDialog(QWidget* parent = NULL);
        ~NameDialog();
    private:
        Ui::NameDialog* ui;
};

#endif // NAMEDIALOG_H
