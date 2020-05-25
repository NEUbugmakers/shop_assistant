#ifndef TRADEFLOWWIDGET_H
#define TRADEFLOWWIDGET_H

#include <QWidget>
#include "T_member.h"

namespace Ui {
class tradeflowwidget;
}

class tradeflowwidget : public QWidget
{
    Q_OBJECT

public:
    explicit tradeflowwidget(QWidget *parent = nullptr);
    ~tradeflowwidget();
    void T_recordsearch();
    void T_recordshow(FILE* fp);
private slots:
    void onBeforeDateChanged(const QDate &date);
    void onAfterDateChanged(const QDate &date);
signals:
    void sendsignal();
private:
    Ui::tradeflowwidget *ui;
    int totalday=0;
    float acost=0.0,asell=0.0,aprofit=0.0,tcost=0.0,tsell=0.0,tprofit=0.0;
    struct T_date* afterdate = nullptr, *beforedate = nullptr, *searchdate;
    int lose=0;
};
//ui->label_acost->setText("");
//ui->label_asell->setText("");
//ui->label_aprofit->setText("");
//ui->label_tcost->setText("");
//ui->label_tsell->setText("");
//ui->label_tprofit->setText("");

#endif // TRADEFLOWWIDGET_H
