#include "sr_query.h"
#include "ui_sr_query.h"

SR_query::SR_query(QWidget *parent,char* tname) :
    QDialog(parent),
    ui(new Ui::SR_query)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"商品名称"<<"商品编码");
    strcpy(name,tname);
    loadInfo();
}

SR_query::~SR_query()
{
    delete ui;
}
void SR_query::loadInfo(){
    B_vector* vector = SR_dataNameFind(name);
    if(vector==NULL){
        QMessageBox::warning(this,"查询失败","商品不存在");
        return;
    }
    ui->tableWidget->setRowCount(vector->_size);
    for(int i = 0; i < vector->_size ; i++){
        C_Goods* goods = SR_dataGet(B_vectorGet(vector,i));
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString(goods->name)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString(goods->code)));
    }
    B_vectorClear(vector);
    free(vector);
}
