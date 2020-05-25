#include "discountwidget.h"
#include "ui_discountwidget.h"
#include "tradeadd.h"
#include <QtDebug>
#include <QDialog>


discountwidget::discountwidget(struct T_discount* T_head,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::discountwidget)
{
    ui->setupUi(this);
    T_discountparent=this;
    ui->pushButton_select->hide();
    ui->pushButton_search->setText("清空");
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"商品编号"<<"商品名称"<<"批次"<<"进价"<<"单价"<<"优惠折扣"<<"售价");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSortingEnabled(true);
//    Mhead=M_head;
    T_discounthead=T_head;
    Ddisplay();
    connect(ui->pushButton_add,&QPushButton::clicked,[=](){
       qDebug() <<"添加" ;
//       tradeadd adddig(T_discounthead,this,this);
//       adddig.exec();
       tradeadd w(T_head,this,this);
       w.exec();
    });
    connect(ui->pushButton_search,&QPushButton::clicked,[=](){
       Dempty();
//       seadlg.exec();
    });
    connect(this,&discountwidget::S_discountadd,[=](){
       qDebug() <<"display" ;
       Ddisplay();
    });
    connect(this,&discountwidget::S_discountsearch,[=](){
       qDebug() <<"search" ;
    });
    connect(ui->pushButton_delete,&QPushButton::clicked,this,&discountwidget::Ddelete);
    connect(ui->pushButton_search,&QPushButton::clicked,this,&discountwidget::Ddelete);
    connect(ui->pushButton_back,&QPushButton::clicked,[=](){
        T_discountsave(T_discounthead);
        emit sendsignal();
        this->close();
    });
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

discountwidget::~discountwidget()
{
    delete ui;
}

void discountwidget::Dempty(){
    struct T_discount* p;
    tdp= nullptr;
    p = T_discounthead->next;
    while(p!=nullptr){
        tdp=p->next;
        free(p);
        p=tdp;
    }
    T_discounthead->next=nullptr;
    T_discountnow=T_discounthead;
    Ddisplay();
}
void discountwidget::Ddelete(){
    QList<QTableWidgetSelectionRange>ranges = ui->tableWidget->selectedRanges();
    int count=ranges.count();
    int deleterow=-1;
    for(int i=0;i<count;i++){
          int topRow=ranges.at(i).topRow();
          int bottomRow=ranges.at(i).bottomRow();
          //for(int i=topRow,j=topRow;j<=bottomRow;j++){
          if(bottomRow>=topRow){
              T_discountdelete(topRow+1, T_discounthead,bottomRow-topRow);
              deleterow=bottomRow-topRow;
          }else deleterow=-1;
    }
   Ddisplay();
}
void discountwidget::Ddisplay(){
    int i=0;
    tdp=T_discounthead;
    ui->tableWidget->setRowCount(0);
    for (; tdp->next != nullptr; i++) {
        ui->tableWidget->setRowCount(i+1);
        tdp = tdp->next;
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(tdp->T_num)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(tdp->T_name)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(tdp->T_batch)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(tdp->T_in, 'f', 2)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(tdp->T_sell, 'f', 2)));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(tdp->T_discount, 'f', 2)));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(tdp->T_out, 'f', 2)));
    }
}
