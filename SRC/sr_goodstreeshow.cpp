#include "sr_goodstreeshow.h"
#include "ui_sr_goodstreeshow.h"
sr_goodstreeshow::sr_goodstreeshow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sr_goodstreeshow)
{
    ui->setupUi(this);
    connect(ui->treeWidget,&QTreeWidget::itemDoubleClicked,this,&sr_goodstreeshow::showInfo);
    connect(ui->replenish,&QPushButton::clicked,this,&sr_goodstreeshow::replenish);
    connect(ui->outofstock,&QPushButton::clicked,this,&sr_goodstreeshow::outofstock);
    connect(ui->addMainSort,&QPushButton::clicked,this,&sr_goodstreeshow::addmainsort);
    connect(ui->rot,&QPushButton::clicked,this,&sr_goodstreeshow::rot);
    connect(ui->replenishremind,&QPushButton::clicked,this,&sr_goodstreeshow::replenishremind);
    connect(ui->query,&QPushButton::clicked,this,&sr_goodstreeshow::query);
    connect(ui->back,&QPushButton::clicked,[=](){
        emit sendsignal();
        this->close();
    });
    update();
    //    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<"666");
    //    ui->treeWidget->addTopLevelItem(item);
    //    item =    new QTreeWidgetItem(QStringList()<<"333");
    //    ui->treeWidget->addTopLevelItem(item);
}

sr_goodstreeshow::~sr_goodstreeshow()
{
    delete ui;
}
void sr_goodstreeshow::showInfo(QTreeWidgetItem *item, int column){
    qDebug()<<"test";
    QTreeWidgetItem* currentItem = ui->treeWidget->currentItem();
    if(currentItem==NULL)return ;
    QString code = currentItem->text(1);
    int len = code.length();
    if(isalpha(code.toUtf8().data()[len-1])){
        qDebug()<<"sorttest"<<code.toUtf8().data();
        sr_sortshow m(this,code.toUtf8().data());
        m.exec();
        update();
    }else{
        qDebug()<<code.toUtf8().data();
        c_goodsinfoshow m(this,code.toUtf8().data());
        m.exec();
        update();
    }
    SR_dataSave();
}
void sr_goodstreeshow::update(){
    ui->treeWidget->clear();
    B_vector* sort = SR_dataSortVector();
    B_list* stack = B_listCreat(sizeof(QTreeWidgetItem*));
    ui->treeWidget->setHeaderLabels(QStringList()<<"商品"<<"编号");

    for(int i =0;i<sort->_size;i++){
        SR_dataBTNode* pos = SR_dataGetSort(B_vectorGet(sort,i));
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<pos->name<<pos->sort);
        if(stack->_size==0){
            ui->treeWidget->addTopLevelItem(item);
            B_listPushBack(stack,&item);
        }
        else{
            while(strlen(B_vectorGet(sort,i))-1<stack->_size)
                B_listRemoveBack(stack);
            if(stack->_size==0){
                i--;
                continue;
            }
            (*(QTreeWidgetItem**)B_listGetBack(stack))->addChild(item);
            B_listPushBack(stack,&item);
        }
        for(int j=0;j<pos->goodsVector->vector->_size;j++){
            C_Goods* goods = (C_Goods*)B_vectorGet(pos->goodsVector->vector,j);
            qDebug()<<QString(goods->code)<<endl<<QString(goods->name);
            item = new QTreeWidgetItem(QStringList()<<QString(goods->name)<<QString(goods->code));
            (*(QTreeWidgetItem**)B_listGetBack(stack))->addChild(item);
        }
    }
    free(sort);
    free(stack);
}
void sr_goodstreeshow::replenish(){
    sr_replenish m;
    m.exec();
    update();
    SR_dataSave();
}
void sr_goodstreeshow::outofstock(){
    c_outofstock m;
    m.exec();
    update();
    SR_dataSave();
}
void sr_goodstreeshow::addmainsort(){
    sr_addnewsort m(this,"");
    m.exec();
    update();
    SR_dataSave();
}
void sr_goodstreeshow::rot(){
    sr_rotwarning w(this);
    w.exec();
}
void sr_goodstreeshow::replenishremind(){
    sr_replenishremind m(this);
    m.exec();
}
void sr_goodstreeshow::query(){
    SR_query w(this,ui->name->text().toUtf8().data());
    w.exec();
}
