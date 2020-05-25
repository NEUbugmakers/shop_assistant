#include "membergood.h"
#include "ui_membergood.h"
#include "T_member.h"
#include <QMessageBox>
#include <QtDebug>
MemberGood::MemberGood(struct Member* mp,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MemberGood)
{

    ui->setupUi(this);
    connect(ui->pushButton_back,&QPushButton::clicked,[=](){
        if(M_back==0){
            this->close();
        } else{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"订单号"<<"进价"<<"售价"<<"消费日期");
            M_memberdisplay();
            M_back=0;
        }
    });
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"订单号"<<"进价"<<"售价"<<"消费日期");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    Mp=mp;
    M_memberdisplay();
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(M_Gooddisplay(int,int)));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSortingEnabled(true);
}
void MemberGood::M_Gooddisplay(int row,int column){
    if (row >= sum)QMessageBox::warning(this,"警告","请选择正确的记录");
    else M_recordsearch(row+1,Mp->M_buy);
}

MemberGood::~MemberGood()
{
    delete ui;
}
void MemberGood::T_recordsearch(int M_number, FILE* fp) {
    char M_first, G_first;
    int T_amount;
    float T_in, T_out;
    int mode = 0;
    char M_num[12], T_num[19], date[9];
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"商品编码"<<"数量"<<"进价"<<"售价");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    while (!feof(fp)) {
        M_first = fgetc(fp);
        if (M_first == '#') {
            M_number--;
            fread(M_num, sizeof(char), 10, fp);
            M_num[10] = '\0';
            fscanf(fp, "%s", date);
            fscanf(fp, "%f", &T_in);
            fscanf(fp, "%f", &T_out);
            if (M_number == 0){
                ui->label->setText(QObject::tr("会员编码：1%1").arg(Mp->M_num));
                ui->label_2->setText(QString("订单编号：%1%2").arg(date).arg(mtp->line,4,10,QLatin1Char('0')));
                QString str1 = "进货金额: ";
                str1.append(QString::number(T_in, 'f', 2));
                ui->label_3->setText(str1);
                str1 = "销售金额:";
                str1.append(QString::number(T_out, 'f', 2));
                ui->label_4->setText(str1);
                ui->label_5->setText(QObject::tr("交易时间：%1").arg(date));
                M_back=1;
            }
            while (!feof(fp)) {
                G_first = fgetc(fp);
                if (G_first == '&') break;
                fscanf(fp, "%d", &T_amount);
                fscanf(fp, "%s", &T_num);
                fscanf(fp, "%f", &T_in);
                fscanf(fp, "%f", &T_out);
                if (M_number == 0) {
                    ui->tableWidget->setRowCount(mode+1);
                    ui->tableWidget->setItem(mode, 0, new QTableWidgetItem(QString("%1").arg(T_num)));
                    ui->tableWidget->setItem(mode, 1, new QTableWidgetItem(QString("%1").arg(T_amount)));
                    ui->tableWidget->setItem(mode, 2, new QTableWidgetItem(QString::number(T_in, 'f', 2)));
                    ui->tableWidget->setItem(mode++, 3, new QTableWidgetItem(QString::number(T_out, 'f', 2)));

                }
            }
        }
        if (M_number == 0)	break;
    }
    if (mode == 0) QMessageBox::warning(this,"警告","未找到对应交易记录");
    fclose(fp);
}
void MemberGood::M_recordsearch(int M_number, struct M_trade* M_head) {
    struct M_trade* p = M_head;
    while (M_number--)p = p->next;
    char* filename = (char*)malloc(40 * sizeof(char));;
    T_datefile(filename, p->M_date);
    mtp=p;
    FILE* fp;
    fp = fopen(filename, "r");
    qDebug()<< filename;
    free(filename);
    if (fp == nullptr) {
        QMessageBox::warning(this,"警告","该天交易记录丢失");
    }
    else T_recordsearch(p->line, fp);
}
void MemberGood::M_memberdisplay(){
    struct M_trade* M_buy=Mp->M_buy;
    char* sexchar[2]={"女","男"};
    char* levelchar[5] = {"注册会员","青铜会员","银牌会员","金牌会员","钻石会员"};
    ui->label->setText(QObject::tr("会员编码：1%1").arg(Mp->M_num));
    ui->label_2->setText(QObject::tr("性别：%1").arg(sexchar[Mp->M_sex-'0']));
    ui->label_3->setText(QObject::tr("会员等级：%1").arg(levelchar[Mp->M_level-'0']));
    QString str1 = QString("消费积分:");
    str1.append(QString::number(Mp->M_money, 'f', 2));
    ui->label_4->setText(str1);
    ui->label_5->setText(QObject::tr("到期时间：%1").arg(Mp->M_date));
    if (M_buy->next== nullptr) {
        QMessageBox::warning(this,"警告","该会员下无交易记录");
    }else {
        int i=0;
        mtp = M_buy;
        while (mtp->next != nullptr) {
              ui->tableWidget->setRowCount(i+1);
             mtp = mtp->next;
             ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(mtp->M_date)));
             ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1%2").arg(mtp->M_date).arg(mtp->line,4,10,QLatin1Char('0'))));
             ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(mtp->T_in, 'f', 2)));
             ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(mtp->T_out, 'f', 2)));
             i++;
             sum++;
        }
    }
}
