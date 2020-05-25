#include "sr_addnewsort.h"
#include "ui_sr_addnewsort.h"

sr_addnewsort::sr_addnewsort(QWidget *parent,char* tsort) :
    QDialog(parent),
    ui(new Ui::sr_addnewsort)
{
    ui->setupUi(this);
    connect(ui->add,&QPushButton::clicked,this,&sr_addnewsort::addNewSort);
    strcpy(sort,tsort);
}

sr_addnewsort::~sr_addnewsort()
{
    delete ui;
}
void sr_addnewsort::addNewSort(){
    if(SR_dataInsertSort(sort,ui->name->text().toUtf8().data()))
        QMessageBox::information(this,"插入分类","插入成功");
    else
        QMessageBox::warning(this,"插入分类","插入失败");
}
