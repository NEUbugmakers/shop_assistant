#include "c_batchesshow.h"
#include "ui_c_batchesshow.h"

c_batchesshow::c_batchesshow(QWidget *parent,char* tcode,int tflag) :
    QDialog(parent),
    ui(new Ui::c_batchesshow)
{
    ui->setupUi(this);
    strcpy(code,tcode);
//    SR_dataSell(code,'A',5,0);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    flag=tflag;
    ui->tableWidget->item(0,0);
    update();
}

c_batchesshow::~c_batchesshow()
{
    delete ui;
}
void c_batchesshow::update(){
    ui->tableWidget->clear();
    C_Goods* goods = SR_dataGet(code);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"批次"<<"数量"<<"进价"<<"入库时间"<<"生产日期"<<"保质期"<<"过期时间"<<"距离过期天数");
    B_list* list;
    if(flag==0)
        list = goods->C_stockInfo;
    else if(flag==1)
        list = goods->C_shelfInfo;
    C_goodsInfo* info;
    ui->tableWidget->setRowCount(list->_size);
    for(int i=0;i<list->_size;i++){
        info = (C_goodsInfo*)B_listGetRank(list,i);
        int count=0;
        qDebug()<<QString(info->batch);
        ui->tableWidget->setItem(i,count++,new QTableWidgetItem(QString(info->batch)));
        ui->tableWidget->setItem(i,count++,new QTableWidgetItem(QString::number(info->C_amount)));
        ui->tableWidget->setItem(i,count++,new QTableWidgetItem(QString::number(info->C_in)));
        ui->tableWidget->setItem(i,count++,new QTableWidgetItem(QString("%1.%2.%3").arg(info->C_entryDate.B_Year,4,10,QLatin1Char('0')).arg(info->C_entryDate.B_Months,2,10,QLatin1Char('0')).arg(info->C_entryDate.B_Day,2,10,QLatin1Char('0'))));
        ui->tableWidget->setItem(i,count++,new QTableWidgetItem(QString("%1.%2.%3").arg(info->C_producedDate.B_Year,4,10,QLatin1Char('0')).arg(info->C_producedDate.B_Months,2,10,QLatin1Char('0')).arg(info->C_producedDate.B_Day,2,10,QLatin1Char('0'))));
        ui->tableWidget->setItem(i,count++,new QTableWidgetItem(QString::number(info->C_shelfDate)+"天"));
        ui->tableWidget->setItem(i,count++,new QTableWidgetItem(QString("%1.%2.%3").arg(info->C_rotDate.B_Year,4,10,QLatin1Char('0')).arg(info->C_rotDate.B_Months,2,10,QLatin1Char('0')).arg(info->C_rotDate.B_Day,2,10,QLatin1Char('0'))));
        ui->tableWidget->setItem(i,count++,new QTableWidgetItem(QString::number(B_DayPassed(B_Time_I(),info->C_rotDate))));
    }
}
