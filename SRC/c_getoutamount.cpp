#include "c_getoutamount.h"
#include "ui_c_getoutamount.h"

c_getoutamount::c_getoutamount(QWidget *parent,char* tcode, char tbatch) :
    QDialog(parent),
    ui(new Ui::c_getoutamount)
{
    ui->setupUi(this);
    connect(ui->outofstock,&QPushButton::clicked,this,&c_getoutamount::outofstock);
    strcpy(code,tcode);
    batch = tbatch;
}

c_getoutamount::~c_getoutamount()
{
    delete ui;
}
void c_getoutamount::outofstock(){
    int amount = ui->amount->text().toInt();
    char tbatch = SR_dataOutOfStockBatch(code,batch,amount);
    if(tbatch!=-1){
        QMessageBox::information(this,"商品出库",QString("出库成功，批次标识符为: ")+tbatch);
    }
    else{
        QMessageBox::warning(this,"商品出库",QString("出库失败,可能为库存不足"));
    }
}
