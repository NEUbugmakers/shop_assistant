#include "tradewidget.h"
#include "ui_tradewidget.h"
#include "memberwidget.h"
#include "membersearch.h"
#include "tradeadd.h"
#include <QtDebug>
#include <QDialog>
#include <QString>
#include <QMessageBox>
tradewidget::tradewidget(int T_sum,struct Trade** T_head,struct Member* M_head,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tradewidget)
{
    ui->setupUi(this);

    T_carthead = (struct T_shopcart*) malloc(sizeof(struct T_shopcart));
    T_carthead->next = nullptr;
    T_cartnow = T_carthead;
    T_tradenow=*T_head;
    M_membernow = (struct Member*) malloc(sizeof(struct Member));
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"商品编号"<<"商品名称"<<"原价"<<"现价"<<"数量"<<"金额");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->label_time->setText(B_timechar());
    ui->tableWidget->setSortingEnabled(true);
    QFont ft;
    ft.setPointSize(16);
    ui->label_money->setFont(ft);
    ui->label_credit->setFont(ft);
    sum=T_sum;
    connect(ui->pushButton_add,&QPushButton::clicked,[=](){
       qDebug() <<"添加" ;
       tradeadd adddig(T_carthead,this,this);
       //(T_shophead,this,this)
       adddig.setWindowTitle("添加购物商品");
       adddig.exec();
    });
    connect(this,&tradewidget::S_tradecartadd,[=](){
       qDebug() <<"display" ;
       goodsum++;
       Tdisplay();
    });
    connect(this,&tradewidget::S_membersearch,[=](){
       qDebug()<<"find it in trade" ;
       T_memberdiscount = M_level(M_membernow);
       Mdisplay();
       Tdisplay();
    });
    M_membernow=M_head;
    T_memberdiscount = M_level(M_membernow);
    Mdisplay();
    Tdisplay();
    connect(ui->pushButton_member,&QPushButton::clicked,[=](){
        membersearch seadlg(&M_membernow,M_head,this,this);
        seadlg.setWindowTitle("会员卡输入");
        Tdisplay();
        seadlg.exec();
    });
    connect(ui->pushButton_delete,&QPushButton::clicked,this,&tradewidget::Tdelete);
    connect(ui->pushButton_settle,&QPushButton::clicked,[=](){
        if(T_carthead->next==NULL){
            QMessageBox::information(this,"结算失败","请在购物车加入商品");
        }else{
            if (T_settle() == 0) {
                QString str1 = "本次交易您共累计积分:";
                str1.append(QString::number(T_credit, 'f', 2));
                QMessageBox::information(this,"结算成功",str1);
                T_save(T_tradenow);
                M_save(M_head);
                T_backsave();
                //printf("今日第%d笔交易 总进价：%.1f 总售价:%.1f", T_sum + 1, T_cost, T_money);
            }else {
                QMessageBox::information(this,"结算失败","本次交易纯利润");
            }
            S_empty();
        }
        M_membernow=M_head;
        T_memberdiscount = M_level(M_membernow);
        Mdisplay();
    });
    connect(ui->pushButton_back,&QPushButton::clicked,[=](){
        S_empty();
        emit sendsignal();
        this->close();
    });
    connect(ui->pushButton_empty,&QPushButton::clicked,[=](){
        S_empty();
    });
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

tradewidget::~tradewidget()
{
    delete ui;
}
void tradewidget::Tdelete(){
    QList<QTableWidgetSelectionRange>ranges = ui->tableWidget->selectedRanges();
    int count=ranges.count();
    int deleterow=-1;
    for(int i=0;i<count;i++){
          int topRow=ranges.at(i).topRow();
          int bottomRow=ranges.at(i).bottomRow();
          if(bottomRow>=topRow){
              T_cartdelete(topRow+1, T_carthead,bottomRow-topRow);
              deleterow=bottomRow-topRow;
          }else deleterow=-1;
    }
   goodsum=goodsum-deleterow-1;
   Tdisplay();
}
void tradewidget::Mdisplay(){
    char *levelchar[6] = {"注册会员","青铜会员","银牌会员","金牌会员","钻石会员","非注册会员"};
    ui->label_num->setText(QObject::tr("会员编码：1%1").arg(M_membernow->M_num));
    ui->label_level->setText(QObject::tr("会员等级：%1").arg(levelchar[M_membernow->M_level-'0']));
    ui->label_money->setText(QObject::tr("当前消费金额：0"));
}
void tradewidget::S_empty(){
    struct T_shopcart* p;
    tcp= nullptr;
    p = T_carthead->next;
    while(p!=nullptr){
        tcp=p->next;
        free(p);
        p=tcp;
    }
    T_carthead->next=nullptr;
    T_cartnow=T_carthead;
    Tdisplay();
}
int tradewidget::T_backsave(){
    FILE* fp;
    char filename[40]="\0";
    fp = fopen("back_list.txt", "r");
    char* datename = (char*)malloc(20 * sizeof(char));;
    T_datefile(datename, NULL);
    if (fp == NULL) {
        fp = fopen("back_list.txt", "w");
        fprintf(fp,"member.txt\ndiscount.txt\nback_list.txt\nroot\ngoods\nSR_sales\nP_list.txt\nW_list.txt\nA_systerm.txt");
    }else{
        while(!feof(fp)){
            fscanf(fp,"%s",filename);
        }
    }
    if(strcmp(filename,datename)!=0){
        qDebug() << "自动创建!";
        fclose(fp);
        fp = fopen("back_list.txt", "a");
        fprintf(fp,"\n%s",datename);
    }
    fclose(fp);
    return 1;
}

void tradewidget::Tdisplay(){
    int i=0;
    T_out = 0.0;
    tcp=T_carthead;
    ui->tableWidget->setRowCount(0);
    for (; tcp->next != nullptr; i++) {
        ui->tableWidget->setRowCount(i+1);
        tcp = tcp->next;
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(tcp->T_num)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(tcp->T_name)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(tcp->T_in, 'f', 2)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(tcp->T_out, 'f', 2)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString("%1").arg(tcp->T_amount)));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(tcp->T_out*tcp->T_amount, 'f', 2)));
        T_out += tcp->T_out*tcp->T_amount;
    }
    QString str1 = "当前购物总金额：";
    str1.append(QString::number(T_out, 'f', 2));
    ui->label_money->setText(str1);
    QString str2 = "购物积分：";
    str2.append(QString::number(T_out*T_memberdiscount, 'f', 2));
    ui->label_credit->setText(str2);
    T_credit=T_out*T_memberdiscount;
}
int tradewidget::T_settle() {
    float T_money = 0.0, T_cost = 0.0, G_discount;
    struct Trade* tp = (struct Trade*)malloc(sizeof(struct Trade));
    struct T_goods* gp = (struct T_goods*)malloc(sizeof(struct T_goods));
    struct T_goods* G_next;
    C_goodsReturn* pp;
    T_tradenow->next = tp;
    tp->next = NULL;
    tp->point = gp;
    tcp=T_carthead;
    strcpy(tp->M_num, M_membernow->M_num);
    char *date=T_timechar();
    strcpy(tp->date, date);
    free(date);
    while (tcp->next != NULL) {
        tcp=tcp->next;
        pp=SR_dataSell(tcp->T_num,tcp->T_batch,tcp->T_amount,0);
        if(pp!=nullptr)free(pp);
        pp=SR_dataSell(tcp->T_num,tcp->T_batch,1,1);
        if(pp!=nullptr){
            free(pp);
        }else if(tcp->T_dis!=nullptr){
            struct T_discount* buffer;
            buffer=tcp->T_dis->next;
            tcp->T_dis->next=buffer->next;
            free(buffer);
        }
        G_next = (struct T_goods*)malloc(sizeof(struct T_goods));
        strcpy(G_next->T_num, tcp->T_num);
        G_next->T_amount = tcp->T_amount;
        G_next->T_in = tcp->T_in * tcp->T_amount;
        G_next->T_out = tcp->T_out * tcp->T_amount*T_memberdiscount;
        gp->next = G_next;
        gp = G_next;
        T_cost += gp->T_in;
        T_money += gp->T_out;
    }
    gp->next = NULL;
    M_membernow->M_money += T_money;
    tp->T_in = T_cost;
    tp->T_out = T_money;
    if(M_membernow->M_sex!='3')M_setlevel(M_membernow, 0);
    T_recordadd(M_membernow->M_buy, ++sum,T_cost,T_money);
    if(M_membernow->M_buy==NULL)qDebug()<<"buy NULL";
    T_profit=T_money-T_cost;
    return 0;
}
