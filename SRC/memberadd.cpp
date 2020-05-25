#include "memberadd.h"
#include "ui_memberadd.h"
#include "T_member.h"
#include <string.h>
#include <QtDebug>
#include <QString>
#include <QMessageBox>
memberadd::memberadd(struct Member* M_head,MemberWidget *Mparent,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::memberadd)
{
    ui->setupUi(this);
    ui->man->setChecked(true);
    ui->lineEdit->setMaxLength(11);
    ui->lineEdit->setPlaceholderText("请在此输入你的手机号");
    //connect(ui->pushButton_delete,&QPushButton::clicked,this,&MemberWidget::Mdelete);
    connect(ui->pushButton_right,&QPushButton::clicked,[=](){
        char * M_num;
        QString NUM;
        NUM  = ui->lineEdit->text();
        QByteArray num_char=NUM.toLatin1();
        M_num = num_char.data();
        struct Member* mp;
        qDebug() << "right";
        qDebug() << M_num;
        int i;
        if(strlen(M_num)!=11){
            QMessageBox::warning(this,"警告","您输入的手机号不满足11位");
        }else if(M_num[0]!='1'){
            QMessageBox::warning(this,"警告","您输入的手机号首位不为1位");
        }else{
            for(i=0;i<11;i++) if(!(M_num[i]>='0'&&M_num[i]<='9'))break;
            if(i==11){
                mp = M_search(M_num + 1, M_head);
                if (mp == nullptr || strcmp(M_num + 1, mp->M_num) != 0) {
                    qDebug() << "查无此账户，自动使用此手机号注册会员";
                    M_add(M_num + 1, mp, T_time(),M_sex);
                    emit Mparent->S_memberadd();
                    this->close();
                }else QMessageBox::warning(this,"警告","您输入的手机号已被注册");
            }else QMessageBox::warning(this,"警告","您输入的手机号首位不为1位");
        }
    });
    connect(ui->woman,&QRadioButton::clicked,[=](){
        M_sex='0';
    });
    connect(ui->man,&QRadioButton::clicked,[=](){
        M_sex='1';
    });
    connect(ui->pushButton_false,&QPushButton::clicked,this,&QDialog::close);
}
memberadd::memberadd(char num[],char sex,struct Member* memberbefore,struct Member* M_head,MemberWidget *Mparent,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::memberadd)
{
    ui->setupUi(this);

    if(sex=='0'){
        ui->woman->setChecked(true);
        M_sex='0';
    }else ui->man->setChecked(true);
    ui->lineEdit->setText(num);
    connect(ui->pushButton_right,&QPushButton::clicked,[=](){
        char M_num[13];
        strcpy(M_num,ui->lineEdit->text().toUtf8().data());
        struct Member* mp;
        qDebug() << "right";

        int i;
        if(strlen(M_num)!=11){
            QMessageBox::warning(this,"警告","您输入的手机号不满足11位");
        }else if(M_num[0]!='1'){
            QMessageBox::warning(this,"警告","您输入的手机号首位不为1位");
        }else{
            for(i=0;i<11;i++) if(!(M_num[i]>='0'&&M_num[i]<='9'))break;
            if(i==11){
                mp = M_search(M_num + 1, M_head);
                if (mp == nullptr || strcmp(M_num + 1, mp->M_num) != 0) {
                    qDebug() << "查无此账户，自动使用此手机号注册会员";
                    struct Member* memberchange=memberbefore->M_next;
                    if(mp!=memberbefore&&mp!=memberchange){
                        memberbefore->M_next=memberchange->M_next;
                        struct Member* addbuffer;
                        addbuffer=mp->M_next;
                        mp->M_next=memberchange;
                        memberchange->M_next=addbuffer;
                    }
                    strcpy(memberchange->M_num,M_num+1);
                    memberchange->M_sex=M_sex;
                }else if(strcmp(M_num+1,memberbefore->M_next->M_num)==0){
                    mp->M_sex=M_sex;
                }else QMessageBox::warning(this,"警告","您输入的手机号已被注册");
                emit Mparent->S_memberedit();
            }

        }
        this->close();
    });
    connect(ui->woman,&QRadioButton::clicked,[=](){
        M_sex = '0';
    });
    connect(ui->man,&QRadioButton::clicked,[=](){
        M_sex = '1';
    });
    connect(ui->pushButton_false,&QPushButton::clicked,this,&QDialog::close);
}

memberadd::~memberadd()
{
    delete ui;
}
