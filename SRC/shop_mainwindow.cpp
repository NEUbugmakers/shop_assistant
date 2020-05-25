#include "shop_mainwindow.h"
#include "ui_shop_mainwindow.h"
#include <QtDebug>
#include <QMessageBox>
shop_mainwindow::shop_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::shop_mainwindow)
{
    ui->setupUi(this);
    int T_sum=0,M_sum=0;
    struct Member* M_head,*M_filehead = (struct Member*)malloc(sizeof(struct Member));
    struct Member* mp;
    struct Trade* T_head = (struct Trade*)malloc(sizeof(struct Trade));
    struct Trade* tp;
    struct T_discount* T_discounthead = (struct T_discount*)malloc(sizeof(struct T_discount));
    mp = M_load(M_filehead,&M_sum);
    if(M_filehead!=NULL)M_head=M_filehead->M_next;
    tp = T_load(T_head,&T_sum);
    T_disload(T_discounthead);
    B_discounthead=T_discounthead;
    FILE* fp;
    fp=fopen("W_list.txt","a");
    if(fp==nullptr) fp=fopen("W_list.txt","w");
    fclose(fp);
    fp=fopen("P_list.txt","a");
    if(fp==nullptr) fp=fopen("P_list.txt","w");
    fclose(fp);
    fp=fopen("A_systerm.txt","a");
    if(fp==nullptr){
        fp=fopen("A_systerm.txt","w");
        fprintf(fp,"******\n25476631496073733496272252804008370815341372\n");
    }
    fclose(fp);
    section_member = new MemberWidget(M_sum,M_head,nullptr);
    section_member->setWindowTitle("会员管理系统");
    connect(section_member,SIGNAL(sendsignal()),this,SLOT(reshow()));
    connect(ui->pushButton_member,&QPushButton::clicked,[=](){
        section_member->Mdisplay();
        section_member->show();
        this->hide();
    });
    section_trade = new tradewidget(T_sum,&tp,M_head,nullptr);
    section_trade->setWindowTitle("收银系统");
    connect(section_trade,SIGNAL(sendsignal()),this,SLOT(reshow()));
    connect(ui->pushButton_trade,&QPushButton::clicked,[=](){
        section_trade->show();
        this->hide();
    });
    section_discount = new discountwidget(T_discounthead,nullptr);
    section_discount->setWindowTitle("优惠管理系统");
    connect(section_discount,SIGNAL(sendsignal()),this,SLOT(reshow()));
    connect(ui->pushButton_discount,&QPushButton::clicked,[=](){
        section_discount->Ddisplay();
        section_discount->show();
        this->hide();
    });
    section_flow = new tradeflowwidget;
    section_flow->setWindowTitle("流水统计系统");
    connect(section_flow,SIGNAL(sendsignal()),this,SLOT(reshow()));
    connect(ui->pushButton_flow,&QPushButton::clicked,[=](){
        section_flow->show();
        this->hide();
    });
    section_shelf = new sr_goodstreeshow;
    section_shelf->setWindowTitle("商品货架库存管理系统");
    connect(section_shelf,SIGNAL(sendsignal()),this,SLOT(reshow()));
    connect(ui->pushButton_shelf,&QPushButton::clicked,[=](){
        section_shelf->show();
        this->hide();
    });
    section_people = new WP_show;
    connect(section_people,SIGNAL(sendsignal()),this,SLOT(reshow()));
    connect(ui->pushButton_people,&QPushButton::clicked,[=](){
        section_people->WP_showChoice(1);
        section_people->show();
        this->hide();
    });
    connect(ui->pushButton_wage,&QPushButton::clicked,[=](){
        section_people->WP_showChoice(2);
        section_people->show();
        this->hide();
    });
    connect(ui->pushButton_backup,&QPushButton::clicked,[=](){
        qDebug()<<"backup";
        if(B_backup()==1){
            QMessageBox::information(this, "备份", "备份成功！");
        }else QMessageBox::warning(this,"警告","备份源文件丢失，仅实现基本备份！");
    });
    connect(ui->pushButton_resume,&QPushButton::clicked,[=](){
        qDebug()<<"backup";
        if(B_resume()==1){
            QMessageBox::information(this, "恢复", "恢复成功！");
        }else QMessageBox::warning(this,"警告","恢复源文件丢失，仅实现基本恢复！");
    });
    connect(ui->pushButton_back,&QPushButton::clicked,[=](){
        emit sendsignal();
        this->close();
    });
}

shop_mainwindow::~shop_mainwindow()
{
    delete ui;
}
void shop_mainwindow::reshow(){
    this->show();
}
