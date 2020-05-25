#include "c_outofstock.h"
#include "ui_c_outofstock.h"

c_outofstock::c_outofstock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::c_outofstock)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget,&QTableWidget::cellDoubleClicked,this,&c_outofstock::outofstock);
    connect(ui->query,&QPushButton::clicked,this,&c_outofstock::query);
}
c_outofstock::~c_outofstock()
{
    delete ui;
}
void c_outofstock::update(){
    ui->tableWidget->clear();
    C_Goods* goods = SR_dataGet(code);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"批次"<<"数量"<<"进价"<<"入库时间"<<"生产日期"<<"保质期"<<"过期时间"<<"距离过期天数");
    B_list* list;
    list = goods->C_stockInfo;
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
void c_outofstock::outofstock(int row,int column){
    qDebug()<<"出货批次"<<ui->tableWidget->item(row,0)->text().toUtf8().data()[0];
    char tbatch = ui->tableWidget->item(row,0)->text().toUtf8().data()[0];
    c_getoutamount m(this,code,tbatch);
    m.exec();
    update();
    this->close();
}
void c_outofstock::query(){
    C_Goods* goods;
    if(isalpha(ui->code->text().toUtf8().data()[0]))
        goods=SR_dataGet(ui->code->text().toUtf8().data());
    else
        goods = SR_dataCodeFind(ui->code->text().toUtf8().data());
    qDebug()<<ui->code->text().toUtf8().data();
    if(goods==NULL){
        QMessageBox::warning(this,"查询失败","商品不存在");
        ui->tableWidget->setRowCount(0);
        return;
    }
    strcpy(code,goods->code);
    update();
}
