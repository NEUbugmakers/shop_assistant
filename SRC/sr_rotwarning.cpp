#include "sr_rotwarning.h"
#include "ui_sr_rotwarning.h"

sr_rotwarning::sr_rotwarning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sr_rotwarning)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"商品名称"<<"商品编号"<<"位置"<<"批次"<<"数量"<<"保质期"<<"过期时间"<<"状态");
    loadInfo();
}

sr_rotwarning::~sr_rotwarning()
{
    delete ui;
}
void sr_rotwarning::loadInfo(){
    C_goodsVector* rotVector = SR_dataGetRot();
    if(rotVector==NULL)
        return;
    C_Goods* goods;
    int row = 0;
    for(int i = 0; i < rotVector->vector->_size;i++){
        goods = (C_Goods*)B_vectorGet( rotVector->vector ,i);
        for(int j=0;j<goods->C_shelfInfo->_size;j++){
            int con=0;
            ui->tableWidget->setRowCount(row+1);
            C_goodsInfo* info = (C_goodsInfo*)B_listGetRank(goods->C_shelfInfo,j);
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString(goods->name)));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString(goods->code)));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString("货架")));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString(info->batch)));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString::number(info->C_amount)));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString::number(info->C_shelfDate)+"天"));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString(B_DateToStr(&info->C_rotDate,'.'))));
            if(B_DayPassed(info->C_rotDate,B_Time_I())>0){
                ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString("已过期")));
            }
            else{
                ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString("即将过期")));
            }
            row++;
        }
        for(int j=0;j<goods->C_stockInfo->_size;j++){
            int con=0;
            ui->tableWidget->setRowCount(row+1);
            C_goodsInfo* info = (C_goodsInfo*)B_listGetRank(goods->C_stockInfo,j);
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString(goods->name)));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString(goods->code)));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString("库存")));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString(info->batch)));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString::number(info->C_amount)));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString::number(info->C_shelfDate)+"天"));
            ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString(B_DateToStr(&info->C_rotDate,'.'))));
            if(B_DayPassed(info->C_rotDate,B_Time_I())>0){
                ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString("已过期")));
            }
            else{
                ui->tableWidget->setItem(row,con++,new QTableWidgetItem(QString("即将过期")));
            }
            row++;
        }
    }
}
