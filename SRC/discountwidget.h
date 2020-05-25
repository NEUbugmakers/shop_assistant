#ifndef DISCOUNTWIDGET_H
#define DISCOUNTWIDGET_H


#include <QWidget>

namespace Ui {
class discountwidget;
}

class discountwidget : public QWidget
{
    Q_OBJECT

public:
    explicit discountwidget(struct T_discount* T_head,QWidget *parent = nullptr);
    ~discountwidget();
    void Ddelete();
    void Ddisplay();
    void Dadd();
    void Dempty();

signals:
    void S_discountadd();
    void S_discountsearch();
    void sendsignal();

private:
    Ui::discountwidget *ui;
    struct T_discount* T_discounthead=nullptr,*T_discountnow=nullptr,*tdp=nullptr;
};

#endif // DISCOUNTWIDGET_H
