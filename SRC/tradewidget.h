#ifndef TRADEWIDGET_H
#define TRADEWIDGET_H

#include <QWidget>
#include "T_member.h"
#include "SR_data.h"
namespace Ui {
class tradewidget;
}

class tradewidget : public QWidget
{
    Q_OBJECT
    void Tdelete();
    void Tdisplay();
    void Tadd();
    void S_empty();
    void Mdisplay();
    int T_settle();

public:
    explicit tradewidget(int T_sum,struct Trade** T_tradenow,struct Member* M_head,QWidget *parent = nullptr);
    int T_backsave();
    ~tradewidget();
signals:
    void S_tradecartadd();
    void S_tradesearch();
    void S_membersearch();
    void sendsignal();

private:
    Ui::tradewidget *ui;
    struct T_shopcart* T_carthead,*T_cartnow,*tcp;
    struct Member* M_membernow=nullptr;
    struct Trade* T_tradenow=nullptr;
    int sum,goodsum;
    float T_in=0.0,T_out=0.0,T_profit = 0.0,T_credit = 0.0,T_memberdiscount = 0.0;
};

#endif // TRADEWIDGET_H
