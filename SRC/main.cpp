#include "memberwidget.h"
#include "T_member.h"
#include "tradewidget.h"
#include "discountwidget.h"
#include "tradeflowwidget.h"
#include "shop_mainwindow.h"
#include "login.h"
#include "SR_data.h"
#include <QApplication>
#include <QtDebug>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
int main(int argc, char *argv[])
{
    SR_dataBTInit();
    SR_dataSalesInit();
    P_fileRead();
    P_pList = P_vectorPick(P_headA);
    W_fileRead();
     W_pList = W_vectorPick(W_headA);
    QApplication a(argc, argv);
//    shop_mainwindow mainwidget(nullptr);
//    mainwidget.show();
//    mainwidget.setWindowTitle("超市小助手");
    login loginwidget;
    loginwidget.show();
    loginwidget.setWindowTitle("登陆界面");
    return a.exec();
}
