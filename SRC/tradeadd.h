#ifndef TRADEADD_H
#define TRADEADD_H

#include <QDialog>
#include "tradewidget.h"
#include "discountwidget.h"
#include "T_member.h"
#include "SR_data.h"
namespace Ui {
class tradeadd;
}

class tradeadd : public QDialog
{
    Q_OBJECT

public:
    explicit tradeadd(struct T_shopcart* T_carthead,tradewidget* Tparent = nullptr,QWidget *parent = nullptr);
    explicit tradeadd(struct T_discount* T_discounthead,discountwidget* Tparent = nullptr,QWidget *parent = nullptr);
    explicit tradeadd(char num[],char batch[],QWidget *parent);
    ~tradeadd();
private:
    Ui::tradeadd *ui;
};

#endif // TRADEADD_H
