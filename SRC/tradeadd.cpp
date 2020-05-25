#include "tradeadd.h"
#include "ui_tradeadd.h"
#include <string.h>
#include <QtDebug>
#include <QString>
#include <QMessageBox>
#include <QtCore/QTextCodec>
tradeadd::tradeadd(struct T_shopcart* T_carthead,tradewidget *Tparent,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tradeadd)
{
    ui->setupUi(this);
    ui->lineEdit_batch->setMaxLength(1);
    connect(ui->pushButton_right,&QPushButton::clicked,[=](){
        char T_num[19],T_batch;
        int T_amount;
        strcpy(T_num,ui->lineEdit_num->text().toUtf8().data());
        T_batch = ui->lineEdit_batch->text().toUtf8().data()[0];
        T_amount = ui->lineEdit_amount->text().toInt();
        C_goodsReturn* pp;
        pp = SR_dataSell(T_num, T_batch,T_amount,1);
        if(pp==nullptr){
            QMessageBox::warning(this,"警告","您输入的商品不存在或数量超出货架数量");
        }else{
            struct T_shopcart* p=(struct T_shopcart*) malloc(sizeof(struct T_shopcart));
            struct T_shopcart* buffer=T_carthead->next;
            T_carthead->next = p;
            p->next = buffer;
            strcpy(p->T_num,T_num);
            qDebug()<<T_num;
            qDebug()<<T_batch;
            p->T_batch = T_batch;
            p->T_amount = T_amount;
            strcpy(p->T_name,pp->name);
            p->T_in=pp->C_in;
            p->T_dis=T_discout(T_num,T_batch);
            if(p->T_dis==nullptr){
                p->T_out=pp->C_out;
            }else{
                p->T_out=pp->C_out*p->T_dis->next->T_discount;
            }
            emit Tparent->S_tradecartadd();
            this->close();
        }
    });
    connect(ui->pushButton_false,&QPushButton::clicked,this,&QDialog::close);
}
tradeadd::tradeadd(struct T_discount* T_discounthead,discountwidget* Tparent,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tradeadd)
{
    ui->setupUi(this);
    ui->label_3->setText("优惠折扣：");
    ui->lineEdit_batch->setMaxLength(1);
    connect(ui->pushButton_right,&QPushButton::clicked,[=](){
        char T_num[20],T_batch;
        float T_scale;
        struct T_discount* p;
        strcpy(T_num,ui->lineEdit_num->text().toUtf8().data());
        T_batch=ui->lineEdit_batch->text().toUtf8().data()[0];
        T_scale = ui->lineEdit_amount->text().toFloat();
        C_goodsReturn* pp;
        pp = SR_dataSell(T_num,T_batch,0,1);
        p=T_discout(T_num,T_batch);
        if(p!=nullptr){
            p->next->T_discount=T_scale;
            p->next->T_out=p->next->T_sell*T_scale;
            emit Tparent->S_discountadd();
            this->close();
        }else{
            if(pp==nullptr){
                QMessageBox::warning(this,"警告","您输入的商品编码错误或批次不存在");
            }else if(T_scale==0.0){
                QMessageBox::warning(this,"警告","请输入优惠折扣");
            }else if(T_scale>1.0){
                QMessageBox::warning(this,"警告","请输入小于1的优惠折扣");
            }else{
                p=(struct T_discount*) malloc(sizeof(struct T_discount));
                struct T_discount* buffer=T_discounthead->next;
                T_discounthead->next = p;
                p->next = buffer;
                strcpy(p->T_num,T_num);
                p->T_batch=T_batch;
                p->T_discount = T_scale;
                p->T_in=pp->C_in;
                p->T_sell=pp->C_out;
                p->T_out=pp->C_out*T_scale;
                strcpy(p->T_name,pp->name);
                emit Tparent->S_discountadd();
                qDebug()<<"YES";
                free(pp);
                this->close();
            }
        }
    });
    connect(ui->pushButton_false,&QPushButton::clicked,this,&QDialog::close);
}
tradeadd::tradeadd(char num[],char batch[],QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tradeadd)
{
    ui->setupUi(this);
    ui->lineEdit_num->setText(num);
    ui->lineEdit_batch->setText(batch);
    ui->label_3->setText("优惠折扣：");
    ui->lineEdit_batch->setMaxLength(1);
    connect(ui->pushButton_right,&QPushButton::clicked,[=](){
        char T_num[20],T_batch;
        float T_scale;
        struct T_discount* p;
        strcpy(T_num,ui->lineEdit_num->text().toUtf8().data());
        T_batch=ui->lineEdit_batch->text().toUtf8().data()[0];
        T_scale = ui->lineEdit_amount->text().toFloat();
        p=T_discout(T_num,T_batch);
        if(p!=nullptr){
            p->next->T_discount=T_scale;
            p->next->T_out=p->next->T_sell*T_scale;
            emit T_discountparent->S_discountadd();
            this->close();
        }else{
            C_goodsReturn* pp;
            pp = SR_dataSell(T_num,T_batch,0,1);
            if(pp==nullptr){
                QMessageBox::warning(this,"警告","您输入的商品编码错误或批次不存在");
            }else if(T_scale==0.0){
                QMessageBox::warning(this,"警告","请输入优惠折扣");
            }else if(T_scale>1.0){
                QMessageBox::warning(this,"警告","请输入小于1的优惠折扣");
            }else{
                p=(struct T_discount*) malloc(sizeof(struct T_discount));
                struct T_discount* buffer=B_discounthead->next;
                B_discounthead->next = p;
                p->next = buffer;
                strcpy(p->T_num,T_num);
                p->T_batch=T_batch;
                p->T_discount = T_scale;
                p->T_in=pp->C_in;
                p->T_sell=pp->C_out;
                p->T_out=pp->C_out*T_scale;
                strcpy(p->T_name,pp->name);
                emit T_discountparent->S_discountadd();
                if(T_discountparent==NULL)qDebug()<<"YES";
                free(pp);
                this->close();
            }
        }

    });
    connect(ui->pushButton_false,&QPushButton::clicked,this,&QDialog::close);
}

tradeadd::~tradeadd()
{
    delete ui;
}
