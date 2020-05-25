#include "sr_addnewgoods.h"
#include "ui_sr_addnewgoods.h"

sr_addnewgoods::sr_addnewgoods(QWidget *parent,char* tsort) :
    QDialog(parent),
    ui(new Ui::sr_addnewgoods)
{
    ui->setupUi(this);
    strcpy(sort,tsort);
    connect(ui->add,&QPushButton::clicked,this,&sr_addnewgoods::add);
}

sr_addnewgoods::~sr_addnewgoods()
{
    delete ui;
}
void sr_addnewgoods::add(){
    C_Goods goods = C_GoodsCreat(sort,(int)(ui->price->text().toFloat()*100)/100.0,ui->name->text().toUtf8().data(),ui->code->text().toUtf8().data());
    if(SR_dataAddNewGoods(&goods)==0){
        QMessageBox::warning(this,QString("插入商品"),QString("插入失败"));
        return;
    }else{
        QMessageBox::information(this,QString("插入商品"),QString("插入成功"));
    }
    SR_dataCodeVectorBuild();
}
