#include "membersearch.h"
#include "ui_membersearch.h"
#include <QMessageBox>

membersearch::membersearch(struct Member** S_head,struct Member* M_head,MemberWidget *Mparent,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::membersearch)
{
    ui->setupUi(this);
    //struct Member* M_search(char num[], struct Member* M_head) {
    //M_head=
//    ui->lineEdit->setMaxLength(11);
    char * M_num;
    QString NUM;
    NUM  = ui->lineEdit->text();
    QByteArray num_char=NUM.toLatin1();
    M_num = num_char.data();
    qDebug() << "right";
    qDebug() << M_num;
    ui->lineEdit->setPlaceholderText("请在此输入手机号");
    connect(ui->pushButton_right,&QPushButton::clicked,[=](){
        char * M_num;
        QString NUM;
        NUM  = ui->lineEdit->text();
        QByteArray num_char=NUM.toLatin1();
        M_num = num_char.data();
        struct Member* mp;
        qDebug() << "right";
        qDebug() << M_num;
        if(ui->checkBox->isChecked()==true){
            if(strlen(M_num)<=11){
                emit Mparent->S_membersearch(M_num);
                this->close();
            }else QMessageBox::warning(this,"警告","您输入的手机号多于11位");
        }else{
            if(strlen(M_num)!=11){
                QMessageBox::warning(this,"警告","您输入的手机号不满足11位");
            }else{
                mp = M_search(M_num + 1, M_head);
                if (mp == nullptr || strcmp(M_num + 1, mp->M_num) != 0) {
                    QMessageBox::warning(this,"警告","未查到此手机号");
                }else{
                    (*S_head)=mp;
                    emit Mparent->S_membersearchaccurate();
                    this->close();
                }
            }
        }



    });
    connect(ui->pushButton_false,&QPushButton::clicked,this,&QDialog::close);
}
membersearch::membersearch(struct Member** S_head,struct Member* M_head,tradewidget *Tparent,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::membersearch)
{
    ui->setupUi(this);
    //struct Member* M_search(char num[], struct Member* M_head) {
    //M_head=
    ui->lineEdit->setMaxLength(11);
    char * M_num;
    QString NUM;
    NUM  = ui->lineEdit->text();
    QByteArray num_char=NUM.toLatin1();
    M_num = num_char.data();
    qDebug() << "right";
    qDebug() << M_num;
    ui->groupBox->setTitle("请在下框输入你的会员号，若没有请点击取消");
    connect(ui->pushButton_false,&QPushButton::clicked,[=](){
        (*S_head)=M_head;
        emit Tparent->S_membersearch();
        this->close();
    });
    connect(ui->pushButton_right,&QPushButton::clicked,[=](){
        char * M_num;
        QString NUM;
        NUM  = ui->lineEdit->text();
        QByteArray num_char=NUM.toLatin1();
        M_num = num_char.data();
        struct Member* mp;
        qDebug() << "right";
        qDebug() << M_num;
        if(strlen(M_num)!=11){
            QMessageBox::warning(this,"警告","您输入的手机号不满足11位");
        }else{
            mp = M_search(M_num + 1, M_head);
            if (mp == nullptr || strcmp(M_num + 1, mp->M_num) != 0) {
                QMessageBox::warning(this,"警告","未查到此手机号");
            }else{
                (*S_head)=mp;
                emit Tparent->S_membersearch();
                this->close();
            }
        }
    });
}

membersearch::~membersearch()
{
    delete ui;
}
