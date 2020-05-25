#include "sr_replenishremind.h"
#include "ui_sr_replenishremind.h"

sr_replenishremind::sr_replenishremind(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sr_replenishremind)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"商品名称"<<"编码"<<"库存剩余"<<"货架剩余"<<"总剩余"<<"七日销量"<<"三十日销量"<<"预计可售天数");
    loadInfo();
}

sr_replenishremind::~sr_replenishremind()
{
    delete ui;
}
void sr_replenishremind::loadInfo(){
    B_vector* vector = SR_dataReplenishRemind();
    char* code;
    if(vector==NULL)
        return;
    ui->tableWidget->setRowCount(vector->_size);
    for(int i = 0;i<vector->_size;i++){
        code = B_vectorGet(vector,i);
        C_Goods* goods = SR_dataGet(code);
        C_sales* sales = SR_dataGetSales(code);
        int con = 0;
        ui->tableWidget->setItem(i,con++,new QTableWidgetItem(QString(goods->name)));
        ui->tableWidget->setItem(i,con++,new QTableWidgetItem(QString(goods->code)));
        ui->tableWidget->setItem(i,con++,new QTableWidgetItem(QString::number(goods->C_shelfTotal)));
        ui->tableWidget->setItem(i,con++,new QTableWidgetItem(QString::number(goods->C_stockTotal)));
        ui->tableWidget->setItem(i,con++,new QTableWidgetItem(QString::number(goods->C_stockTotal + goods->C_shelfTotal)));
        ui->tableWidget->setItem(i,con++,new QTableWidgetItem(QString::number(C_goodsGetMultiSales(sales,7))));
        ui->tableWidget->setItem(i,con++,new QTableWidgetItem(QString::number(C_goodsGetMultiSales(sales,30))));
        ui->tableWidget->setItem(i,con++,new QTableWidgetItem(QString::number((goods->C_stockTotal + goods->C_shelfTotal)*30/C_goodsGetMultiSales(sales,30))));
    }
}
