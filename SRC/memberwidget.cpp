#include "memberwidget.h"
#include "ui_memberwidget.h"
#include "memberadd.h"
#include "membersearch.h"
#include "membergood.h"
#include "memberchoose.h"
#include <QtDebug>
#include <QDialog>

MemberWidget::MemberWidget(int M_sum,struct Member* M_head,QWidget *parent ) :
    QWidget(parent),
    ui(new Ui::MemberWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"会员编号"<<"性别"<<"会员等级"<<"消费金额"<<"到期时间");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSortingEnabled(true);
    Mhead=M_head;
    Shead=(struct Member*) malloc(sizeof(struct Member));
    sum=M_sum;
    Mdisplay();

    connect(ui->pushButton_add,&QPushButton::clicked,[=](){
       qDebug() <<"添加" ;
       memberadd adddig(M_head,this,this);
       adddig.setWindowTitle("会员添加");
       adddig.exec();
    });
    connect(ui->pushButton_select,&QPushButton::clicked,[=](){
       qDebug() <<"筛选" ;
       memberchoose selectdlg(T_memberchoose,M_head,this,this);
       selectdlg.setWindowTitle("会员筛选");
       selectdlg.exec();
    });
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(M_memberedit(int,int)));
    connect(ui->pushButton_search,&QPushButton::clicked,[=](){
       qDebug() <<"搜索" ;
       membersearch seadlg(&Shead,Mhead,this,this);
       seadlg.setWindowTitle("会员交易记录查找");
       seadlg.exec();
    });
    connect(this,&MemberWidget::S_memberadd,[=](){
       qDebug() <<"display" ;
       Mdisplay();
    });
    connect(this,&MemberWidget::S_memberedit,[=](){
       qDebug() <<"display" ;
       Mdisplay();
    });
    connect(this,&MemberWidget::S_membersearchaccurate,[=](){
       qDebug() <<"search" ;
       MemberGood goodlg(Shead,nullptr);
       goodlg.exec();
    });
    connect(this,SIGNAL(S_membersearch(char*)),this,SLOT(S_searchfuzzy(char*)));
    connect(this,&MemberWidget::S_memberchoose,[=](){
       Mselect();
       Mchoosedisplay();
       qDebug() <<"select" ;
    });
    connect(ui->pushButton_delete,&QPushButton::clicked,this,&MemberWidget::Mdelete);
    connect(ui->pushButton_back,&QPushButton::clicked,[=](){
        M_save(Mhead);
        emit sendsignal();
        this->close();
    });
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MemberWidget::~MemberWidget()
{
    delete ui;
}
void MemberWidget::S_searchfuzzy(char* number){
    qDebug()<<"number:"<<number;
    char *levelchar[5] = {"注册会员","青铜会员","银牌会员","金牌会员","钻石会员"};
    int row=0,len=strlen(number),i,j;
    char M_num[15]="1";
    qDebug()<<len;
    mp=Mhead;
    qDebug()<< "cmp:"<<strcmp(M_num,number);
    ui->tableWidget->setRowCount(0);
    while(mp->M_next != nullptr) {
        mp = mp->M_next;
        strcpy(M_num+1,mp->M_num);
        for(i=0;i<=11-len;i++){
            for(j=0;j<len;j++){
                if(M_num[j+i]!=number[j])break;
            }
            if(j==len){
                ui->tableWidget->setRowCount(row+1);
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(M_num)));
                if(mp->M_sex=='0'){
                    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString("女")));
                }else ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString("男")));
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString("%1").arg(levelchar[mp->M_level-'0'])));
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(mp->M_money, 'f', 1)));
                ui->tableWidget->setItem(row++, 4, new QTableWidgetItem(QString("%1").arg(mp->M_date)));
                break;
            }
        }
    }
}
void MemberWidget::M_memberedit(int row,int column){
    if(editmode==0){
        int i=row;
        struct Member* memberbefore=Mhead;
        while(memberbefore!=NULL&&i--){
            memberbefore=memberbefore->M_next;
        }
        qDebug()<<memberbefore->M_num;
        memberadd adddlg(ui->tableWidget->item(row,0)->text().toUtf8().data(),memberbefore->M_next->M_sex,memberbefore,Mhead,this,this);
        adddlg.exec();
    }
}
void MemberWidget::Mdisplay(){
    int i=0;
    char *levelchar[5] = {"注册会员","青铜会员","银牌会员","金牌会员","钻石会员"};
    mp=Mhead;
    ui->tableWidget->setRowCount(0);
    for (; mp->M_next != nullptr; i++) {
        ui->tableWidget->setRowCount(i+1);
        mp = mp->M_next;
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("1%1").arg(mp->M_num)));
        if(mp->M_sex=='0'){
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("女")));
        }else ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("男")));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(levelchar[mp->M_level-'0'])));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(mp->M_money, 'f', 1)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString("%1").arg(mp->M_date)));
    }
    editmode=0;
}
void MemberWidget::Madd(){

}
void MemberWidget::Mdelete()
{
    QList<QTableWidgetSelectionRange>ranges = ui->tableWidget->selectedRanges();
    int count=ranges.count();
    int deleterow=-1;
    for(int i=0;i<count;i++){
          int topRow=ranges.at(i).topRow();
          int bottomRow=ranges.at(i).bottomRow();
          //for(int i=topRow,j=topRow;j<=bottomRow;j++){
          if(bottomRow>=topRow){
              M_delete(topRow+1, Mhead,bottomRow-topRow);
              deleterow=bottomRow-topRow;
          }else deleterow=-1;
    }
   sum=sum-deleterow-1;
   Mdisplay();
}
void MemberWidget::Mselect(){
    int datecompare;
    char* date=T_timechar();
    Mchoose = (struct Member_choose*) malloc(sizeof(struct Member_choose));
    mcp=Mchoose;
    mp=Mhead;
    while(mp->M_next!=NULL){
        mp=mp->M_next;
        if(T_memberchoose[5]+T_memberchoose[6]==2){
            if(T_memberchoose[mp->M_level-'0']==1&&mp->M_money>=T_memberchoose[7]&&mp->M_money<=T_memberchoose[8]){
                qDebug()<<"choose";
                mcp->M_next=(struct Member_choose*) malloc(sizeof(struct Member_choose));
                mcp=mcp->M_next;
                mcp->M_point=mp;
            }
        }else{
            datecompare=strcmp(mp->M_date,date);
            if((datecompare>=0&&T_memberchoose[5]==1)||(datecompare<0&&T_memberchoose[6]==1)){
                if(T_memberchoose[mp->M_level-'0']==1&&mp->M_money>=T_memberchoose[7]&&mp->M_money<=T_memberchoose[8]){
                    mcp->M_next=(struct Member_choose*) malloc(sizeof(struct Member_choose));
                    mcp=mcp->M_next;
                    mcp->M_point=mp;
                }
            }
        }
    }
    mcp->M_next=NULL;
    free(date);
}
void MemberWidget::Mchoosedisplay(){
    int i=0;
    char *levelchar[5] = {"注册会员","青铜会员","银牌会员","金牌会员","钻石会员"};
    mcp=Mchoose;
    qDebug()<<"Choosedisplay";
    ui->tableWidget->setRowCount(0);
    for (; mcp->M_next != NULL; i++) {
        ui->tableWidget->setRowCount(i+1);
        Mchoose=mcp;
        mcp = mcp->M_next;
        mp=mcp->M_point;
        free(Mchoose);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("1%1").arg(mp->M_num)));
        if(mp->M_sex=='0'){
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("女")));
        }else ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("男")));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(levelchar[mp->M_level-'0'])));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(mp->M_money, 'f', 2)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString("%1").arg(mp->M_date)));
    }
    qDebug()<<i;
    free(mcp);
    editmode=1;
}

