#include "sr_replenish.h"
#include "ui_sr_replenish.h"

sr_replenish::sr_replenish(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sr_replenish)
{
    ui->setupUi(this);

    connect(ui->query,&QPushButton::clicked,this,&sr_replenish::query);
    connect(ui->add,&QPushButton::clicked,this,&sr_replenish::add);
}

sr_replenish::~sr_replenish()
{
    delete ui;
}
void sr_replenish::query(){
    C_Goods* goods;
    if(isalpha(ui->code->text().toUtf8().data()[0]))
        goods=SR_dataGet(ui->code->text().toUtf8().data());
    else
        goods = SR_dataCodeFind(ui->code->text().toUtf8().data());
    qDebug()<<ui->code->text().toUtf8().data();
    if(goods==NULL){
        QMessageBox::warning(this,"查询失败","商品不存在");
        return;
    }
    ui->day->setText(QString::number(goods->C_shelfDate_Pre));
    ui->name->setText(QString(goods->name));
}
void sr_replenish::add(){
    C_goodsInfo info;
    info.C_amount=ui->amount->text().toInt();
    B_DateSet(&info.C_producedDate,ui->produce->date().year(),ui->produce->date().month(),ui->produce->date().day());
    info.C_entryDate = B_Time_I();
    info.C_shelfDate = ui->year->text().toInt()*365 + ui->month->text().toInt()*30 + ui->day->text().toInt();
    info.C_in = ui->price->text().toFloat();
    char batch;
    if(isalpha(ui->code->text().toUtf8().data()[0])){
        batch =SR_dataReplenishGoods(ui->code->text().toUtf8().data(),&info);
    }
    else{
        C_Goods* goods = SR_dataCodeFind(ui->code->text().toUtf8().data());
               if(goods!=NULL)
                   batch = SR_dataReplenishGoods(goods->code,&info);
               else
                   batch = -1;
    }
    if(batch!=-1)
        QMessageBox::information(this,"入库成功",QString("入库成功，批次编码为：")+batch);
    else
        QMessageBox::warning(this,"入库失败","入库失败");
}
