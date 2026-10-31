#ifndef COMPANYPAGE_H
#define COMPANYPAGE_H
#include <QWidget>

namespace Ui{
    class CompanyPage;
}

class CompanyPage :public QWidget{
    Q_OBJECT
    public:
        CompanyPage(QWidget* parent = NULL);
        ~CompanyPage();
    signals:
        void backRequested();
    private:
        Ui::CompanyPage* ui;
};

#endif // COMPANYPAGE_H
