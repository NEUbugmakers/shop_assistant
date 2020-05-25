#include "c_goodsinfoshow.h"
#include "ui_c_goodsinfoshow.h"

c_goodsinfoshow::c_goodsinfoshow(QWidget *parent,char* tcode) :
    QDialog(parent),
    ui(new Ui::c_goodsinfoshow)
{
    ui->setupUi(this);
    C_Goods* pos;
    if(isalpha(tcode[0])){//判断是纯数字编码还是含分类编码
        pos = SR_dataGet(tcode);
    }else{
        pos = SR_dataCodeFind(tcode);
    }
    strcpy(code,pos->code);
    loadInfo();
    connect(ui->remove,&QPushButton::clicked,this,&c_goodsinfoshow::remove);
    connect(ui->save,&QPushButton::clicked,this,&c_goodsinfoshow::save);
    connect(ui->showShelf,&QPushButton::clicked,this,&c_goodsinfoshow::shelf);
    connect(ui->showStock,&QPushButton::clicked,this,&c_goodsinfoshow::stock);
    connect(ui->sales,&QPushButton::clicked,this,&c_goodsinfoshow::sales);
}

c_goodsinfoshow::~c_goodsinfoshow()
{
    delete ui;
}
void c_goodsinfoshow::loadInfo(){
    C_Goods* pos;
    pos = SR_dataGet(code);
    ui->code->setText(pos->code);
    ui->goodsName->setText(pos->name);
    ui->shelfNum->setText(QString("%1").arg(pos->C_shelfTotal));
    ui->stockNum->setText(QString("%1").arg(pos->C_stockTotal));
    ui->price->setText(QString("%1").arg(pos->C_out));
}
void c_goodsinfoshow::remove(){
    SR_dataRemoveGoods(code);
    this->close();
}
void c_goodsinfoshow::save(){
   C_Goods* goods = SR_dataGet(code);
//   strcpy(goods->name,ui->goodsName->text().toUtf8().data());
//   goods->C_out = ui->price->text().toDouble();
   if(C_goodsModify(goods,ui->goodsName->text().toUtf8().data(),ui->price->text().toDouble())){
       QMessageBox::information(this,QString("保存"),QString("保存成功"));
   }else{
       QMessageBox::information(this,QString("保存"),QString("保存失败"));
   }
   this->close();
}
void c_goodsinfoshow::shelf(){
    c_batchesshow m(this,code,1);
    m.exec();
    loadInfo();
}
void c_goodsinfoshow::stock(){
    c_batchesshow m(this,code,0);
    m.exec();
    loadInfo();
}
void c_goodsinfoshow::sales(){
    c_salesshow m(this,code);
    m.exec();
}
