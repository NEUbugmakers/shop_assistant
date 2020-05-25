#ifndef SHOP_MAINWINDOW_H
#define SHOP_MAINWINDOW_H

#include <QMainWindow>
#include "memberwidget.h"
#include "tradewidget.h"
#include "discountwidget.h"
#include "tradeflowwidget.h"//你在这里加你的头文件widget
#include "sr_goodstreeshow.h"
#include "wp_show.h"
#include "b_backup.h"
namespace Ui {
class shop_mainwindow;
}

class shop_mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit shop_mainwindow(QWidget *parent = nullptr);
    ~shop_mainwindow();
public slots:
    void reshow();

signals:
    void sendsignal();

private:
    Ui::shop_mainwindow *ui;
    MemberWidget *section_member;
    tradewidget *section_trade;
    discountwidget *section_discount;
    tradeflowwidget *section_flow;
    sr_goodstreeshow *section_shelf;
    WP_show *section_people;
};

#endif // SHOP_MAINWINDOW_H
