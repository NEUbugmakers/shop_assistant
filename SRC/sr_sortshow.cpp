#include "sr_sortshow.h"
#include "ui_sr_sortshow.h"

sr_sortshow::sr_sortshow(QWidget *parent,char* tcode) :
    QDialog(parent),
    ui(new Ui::sr_sortshow)
{
    ui->setupUi(this);
    connect(ui->addNewGoods,&QPushButton::clicked,this,&sr_sortshow::addNewGoods);
    connect(ui->addNewSort,&QPushButton::clicked,this,&sr_sortshow::addNewSort);
    connect(ui->removeSort,&QPushButton::clicked,this,&sr_sortshow::remove);
    connect(ui->save,&QPushButton::clicked,this,&sr_sortshow::save);
    strcpy(code,tcode);
    loadInfo();
}

sr_sortshow::~sr_sortshow()
{
    delete ui;
}
void sr_sortshow::loadInfo(){
    SR_dataBTNode* pos = SR_dataGetSort(code);
    ui->code->setText(pos->sort);
    ui->name->setText(pos->name);
    ui->goodsNum->setText(QString("%1").arg(pos->goodsVector->vector->_size));
    ui->sortNum->setText(QString("%1").arg(pos->child->_size));
}
void sr_sortshow::addNewGoods(){
    sr_addnewgoods m(this,code);
    m.exec();
}
void sr_sortshow::addNewSort(){
    sr_addnewsort m(this,code);
    m.exec();
}
void sr_sortshow::remove(){//出现过一次崩溃问题
    SR_dataBTNode* node = SR_dataGetSort(code);
    SR_dataRemoveBTNode(node);
    QMessageBox::information(this,"删除","删除成功");
    this->close();
}
void sr_sortshow::save(){
    SR_dataBTNode* node = SR_dataGetSort(code);
    if(SR_dataSortEdit(node,ui->name->text().toUtf8().data())==0){
        QMessageBox::warning(this,"保存","保存失败");
    }else{
        QMessageBox::information(this,"保存","保存成功");
    }
}
