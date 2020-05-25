#include "tradeflowwidget.h"
#include "ui_tradeflowwidget.h"
#include <QtDebug>
#include <QDialog>
#include <QString>
#include <QMessageBox>
tradeflowwidget::tradeflowwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tradeflowwidget)
{
    ui->setupUi(this);
    beforedate = T_time();
    afterdate = T_time();
    searchdate = (struct T_date*)malloc(sizeof(struct T_date));
    ui->pushButton_chart->hide();
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"日期"<<"总成本"<<"总销售额"<<"总利润"<<"交易量");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSortingEnabled(true);

    ui->dateEdit_before->setDateTime(QDateTime::currentDateTime());
    ui->dateEdit_before->setCalendarPopup(true);
    ui->dateEdit_after->setDateTime(QDateTime::currentDateTime());
    ui->dateEdit_after->setCalendarPopup(true);
    connect(ui->pushButton_search,&QPushButton::clicked,[=](){
//       QDebug()<<ui->dateEdit_before
    });
    connect(ui->dateEdit_before, SIGNAL(dateChanged(QDate)), this, SLOT(onBeforeDateChanged(QDate)));
    connect(ui->dateEdit_after, SIGNAL(dateChanged(QDate)), this, SLOT(onAfterDateChanged(QDate)));


    connect(ui->pushButton_search,&QPushButton::clicked,[=](){
        if(T_comparetime(*beforedate,*afterdate)){
            tcost=0.0;
            tsell=0.0;
            tprofit=0.0;
            totalday=0;
            lose=0;
            T_cpytime(searchdate,beforedate);
            while(T_comparetime(*searchdate,*afterdate)){
                T_recordsearch();
                T_DayLater(searchdate,1);
            }
            ui->label_acost->setText(QString::number(tcost, 'f', 2));
            ui->label_asell->setText(QString::number(tsell, 'f', 2));
            ui->label_aprofit->setText(QString::number(tprofit, 'f', 2));
            if(totalday!=0){
                ui->label_tcost->setText(QString::number(tcost/totalday, 'f', 2));
                ui->label_tsell->setText(QString::number(tsell/totalday, 'f', 2));
                ui->label_tprofit->setText(QString::number(tprofit/totalday, 'f', 2));
            }else{
                QMessageBox::warning(this,"警告","全部交易记录丢失");
                lose=0;
            }
            if(lose==1)QMessageBox::warning(this,"警告","部分交易记录丢失");
        }else QMessageBox::warning(this,"警告","您输入的起始日期应在终止日期前");
    });
    connect(ui->pushButton_back,&QPushButton::clicked,[=](){
        emit sendsignal();
        this->close();
    });
}
void tradeflowwidget::T_recordshow(FILE* fp) {
    char M_first, G_first;
    int T_amount,sellsum=0;
    float T_in, T_out,dcost=0.0,dsell=0.0;
    char M_num[12], T_num[19], date[9];
    while (!feof(fp)) {
        M_first = fgetc(fp);
        if (M_first == '#') {
            sellsum++;
            fread(M_num, sizeof(char), 10, fp);
            M_num[10] = '\0';
            fscanf(fp, "%s", date);
            fscanf(fp, "%f", &T_in);
            fscanf(fp, "%f", &T_out);
            dcost+=T_in;
            dsell+=T_out;
            while (!feof(fp)) {
                G_first = fgetc(fp);
                if (G_first == '&') break;
                fscanf(fp, "%d", &T_amount);
                fscanf(fp, "%s", &T_num);
                fscanf(fp, "%f", &T_in);
                fscanf(fp, "%f", &T_out);
            }

        }
    }
    ui->tableWidget->setItem(totalday, 1, new QTableWidgetItem(QString::number(dcost, 'f', 2)));
    ui->tableWidget->setItem(totalday, 2, new QTableWidgetItem(QString::number(dsell, 'f', 2)));
    ui->tableWidget->setItem(totalday, 3, new QTableWidgetItem(QString::number(dsell-dcost, 'f', 2)));
    ui->tableWidget->setItem(totalday++, 4, new QTableWidgetItem(QString("%1").arg(sellsum)));
    tcost+=dcost;
    tsell+=dsell;
    tprofit+=(dsell-dcost);
}
void tradeflowwidget::T_recordsearch() {
    char* timechar = B_timeTochar(searchdate->T_Year,searchdate->T_Months,searchdate->T_Day);
    char* filename = (char*)malloc(40 * sizeof(char));
    T_datefile(filename, timechar);
    FILE* fp;
    fp = fopen(filename, "r");
    qDebug()<<filename;
    if (fp == nullptr) {
        lose=1;
    }else{
        ui->tableWidget->setRowCount(totalday+1);
        ui->tableWidget->setItem(totalday, 0, new QTableWidgetItem(QString("%1").arg(timechar)));
        T_recordshow(fp);
    }
    free(timechar);
}
tradeflowwidget::~tradeflowwidget()
{
    delete ui;
}
void tradeflowwidget::onBeforeDateChanged(const QDate &date) {
    beforedate->T_Year =date.year();
    beforedate->T_Months = date.month();
    beforedate->T_Day = date.day();
}
void tradeflowwidget::onAfterDateChanged(const QDate &date) {
    afterdate->T_Year =date.year();
    afterdate->T_Months = date.month();
    afterdate->T_Day = date.day();
}
