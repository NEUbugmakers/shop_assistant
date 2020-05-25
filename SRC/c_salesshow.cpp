#include "c_salesshow.h"
#include "ui_c_salesshow.h"

c_salesshow::c_salesshow(QWidget *parent,char* tcode) :
    QDialog(parent),
    ui(new Ui::c_salesshow)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    strcpy(code,tcode);
    loadInfo();
}

c_salesshow::~c_salesshow()
{
    delete ui;
}
void c_salesshow::loadInfo(){
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"日期"<<"销量");
    C_sales* sales = SR_dataGetSales(code);
    C_Goods* goods = SR_dataGet(code);
    ui->code->setText(QString(goods->code));
    ui->name->setText(QString(goods->name));
    if(sales==NULL){//判断商品是否存在
        ui->salesofweek->setText(QString::number(0));
        ui->salesofmonth->setText(QString::number(0));
    }
    else{
        ui->salesofweek->setText(QString::number(C_goodsGetMultiSales(sales,7)));
        ui->salesofmonth->setText(QString::number(C_goodsGetMultiSales(sales,30)));
        ui->tableWidget->setRowCount(sales->salesinfo->_size);
        for(int i=sales->salesinfo->_size-1,j=0;i>=0;i--){
            int coun=0;
            C_salesNode* node = (C_salesNode*)B_vectorGet(sales->salesinfo,i);
            ui->tableWidget->setItem(j,coun++,new QTableWidgetItem(QString(B_DateToStr(&node->date,'.'))));
            ui->tableWidget->setItem(j,coun++,new QTableWidgetItem(QString::number(node->amount)));
            j++;
        }
    }
}
