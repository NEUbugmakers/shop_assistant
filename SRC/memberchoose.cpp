#include "memberchoose.h"
#include "ui_memberchoose.h"
#include <QMessageBox>
memberchoose::memberchoose(int memberchoose[],struct Member* M_head,MemberWidget *Mparent,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::memberchoose)
{
    ui->setupUi(this);
//    connect(ui->woman,&QRadioButton::clicked,[=](){
//        M_sex='0';
//    });
//    connect(ui->checkBox_0,&Check)
    ui->lineEdit_down->setMaxLength(6);
    ui->lineEdit_up->setMaxLength(6);
    ui->lineEdit_down->setPlaceholderText("0");
    ui->lineEdit_up->setPlaceholderText("999999");
    if(memberchoose[0]==1) ui->checkBox_0->setChecked(true);
    if(memberchoose[1]==1) ui->checkBox_1->setChecked(true);
    if(memberchoose[2]==1) ui->checkBox_2->setChecked(true);
    if(memberchoose[3]==1) ui->checkBox_3->setChecked(true);
    if(memberchoose[4]==1) ui->checkBox_4->setChecked(true);
    if(memberchoose[5]==1) ui->checkBox_5->setChecked(true);
    if(memberchoose[6]==1) ui->checkBox_6->setChecked(true);
    if(memberchoose[7]!=-1) ui->lineEdit_down->setText(QString::number(memberchoose[7]));
    if(memberchoose[8]!=99999999) ui->lineEdit_up->setText(QString::number(memberchoose[8]));
    connect(ui->pushButton_right,&QPushButton::clicked,[=](){
        if(ui->checkBox_0->isChecked()){
            memberchoose[0]=1;
        }else memberchoose[0]=0;
        if(ui->checkBox_1->isChecked()){
            memberchoose[1]=1;
        }else memberchoose[1]=0;
        if(ui->checkBox_2->isChecked()){
            memberchoose[2]=1;
        }else memberchoose[2]=0;
        if(ui->checkBox_3->isChecked()){
            memberchoose[3]=1;
        }else memberchoose[3]=0;
        if(ui->checkBox_4->isChecked()){
            memberchoose[4]=1;
        }else memberchoose[4]=0;
        if(ui->checkBox_5->isChecked()){
            memberchoose[5]=1;
        }else memberchoose[5]=0;
        if(ui->checkBox_6->isChecked()){
            memberchoose[6]=1;
        }else memberchoose[6]=0;
        char number[10];
        memberchoose[7]=ui->lineEdit_down->text().toInt();
        memberchoose[8]=ui->lineEdit_up->text().toInt();
        if(memberchoose[8]==0){
            strcpy(number,ui->lineEdit_up->text().toUtf8().data());
            if(strlen(number)==0){
                memberchoose[8]=99999999;
            }
        }
        if(memberchoose[7]<0){
            QMessageBox::warning(this,"警告","请输入非负数");
        }else if(memberchoose[7]>memberchoose[8]){
            QMessageBox::warning(this,"警告","后面的数应该大于等于前面的数");
        }else{
            this->close();
            emit Mparent->S_memberchoose();
        }
    });
    connect(ui->pushButton_false,&QPushButton::clicked,this,&QDialog::close);
    connect(ui->pushButton_cancel,&QPushButton::clicked,[=](){
        memberchoose[0]==1;
        ui->checkBox_0->setChecked(true);
        memberchoose[1]==1;
        ui->checkBox_1->setChecked(true);
        memberchoose[2]==1;
        ui->checkBox_2->setChecked(true);
        memberchoose[3]==1;
        ui->checkBox_3->setChecked(true);
        memberchoose[4]==1;
        ui->checkBox_4->setChecked(true);
        memberchoose[5]==1;
        ui->checkBox_5->setChecked(true);
        memberchoose[6]==1;
        ui->checkBox_6->setChecked(true);
        memberchoose[7]==-1;
        memberchoose[8]==99999999;
        ui->lineEdit_down->setText("");
        ui->lineEdit_up->setText("");
    });

}

memberchoose::~memberchoose()
{
    delete ui;
}
