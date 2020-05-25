#include "w_adddata.h"
#include "ui_w_adddata.h"

W_addData::W_addData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::W_addData)
{
    ui->setupUi(this);
    this->setWindowTitle("添加工资信息");
    ui->W_date->setDateTime(QDateTime::currentDateTime());
    ui->W_name->setPlaceholderText("请输入不超过四个字的中文或英文");
    ui->W_name->setMaxLength(4);
    ui->W_name->setClearButtonEnabled(true);


    ui->W_num->setPlaceholderText("10000~99999");
    ui->W_num->setMaxLength(5);
    ui->W_num->setClearButtonEnabled(true);
//    ui->W_num->QIntValidator(this);
//    ui->W_num.setRange(1,99);
//    ui->W_num->setValidator(QIntValidator);

    QRegExp rx("^\\d+(\\.\\d+)?$");
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    ui->W_basicWage->setPlaceholderText("非负浮点数");
    ui->W_basicWage->setValidator(pReg);
    ui->W_basicWage->setClearButtonEnabled(true);


    ui->W_postWage->setPlaceholderText("非负浮点数");
    ui->W_postWage->setValidator(pReg);
    ui->W_postWage->setClearButtonEnabled(true);

    ui->W_reward->setPlaceholderText("非负浮点数");
    ui->W_reward->setValidator(pReg);
    ui->W_reward->setClearButtonEnabled(true);

    ui->W_netWage->setPlaceholderText("非负浮点数");
    ui->W_netWage->setValidator(pReg);
    ui->W_netWage->setClearButtonEnabled(true);

    ui->W_fund->setPlaceholderText("非负浮点数");
    ui->W_fund->setValidator(pReg);
    ui->W_fund->setClearButtonEnabled(true);



    ui->W_houseReform->setPlaceholderText("非负浮点数");
    ui->W_houseReform->setValidator(pReg);
    ui->W_houseReform->setClearButtonEnabled(true);

    ui->W_payedWage->setPlaceholderText("非负浮点数");
    ui->W_payedWage->setValidator(pReg);
    ui->W_payedWage->setClearButtonEnabled(true);


    W_initial(&W_st);//初始化接受结构体
//    int y=0,m=0;
//    QString strYear = ui->W_date->sectionText(QDateTimeEdit::YearSection);
//    QString strMonth = ui->W_date->sectionText(QDateTimeEdit::MonthSection);
//    W_st.W_code =
//    ui->W_date->setDateTime(QDateTime::currentDateTime());

    ui->W_date->setDisplayFormat("yyyy.M");
    ui->W_date->setMaximumDate(QDate::currentDate().addDays(0));
    ui->W_date->setCalendarPopup(true);
    connect(ui->W_date, SIGNAL(dateChanged(QDate)), this, SLOT(onDateChanged(QDate)));

//    connect(ui->W_save,&QPushButton::clicked,[=](){
//        emit WP_update();
//        this->close();
//    });

}

W_addData::~W_addData()
{
    delete ui;
}

void W_addData::on_W_preview_clicked()
{
    this->close();
}

void W_addData::onDateChanged(const QDate &date) {//编号录入
    int year,mon;
    QString strYear = ui->W_date->sectionText(QDateTimeEdit::YearSection);
    QString strMonth = ui->W_date->sectionText(QDateTimeEdit::MonthSection);
    year = strYear.toInt();
    mon = strMonth.toInt();
    int WP_tointDate = year*100+mon;
    W_st.W_code =WP_tointDate;
}



void W_addData::on_W_save_clicked()//保存按钮  **完成,未测试
{

    QString name,basicWage,netWage,houseReform,fund,reward,num,postWage,payedWage;
    QString strNumber = ui->W_num->text();
    basicWage = ui->W_basicWage->text();
    netWage = ui->W_netWage->text();
    houseReform = ui->W_houseReform->text();
    fund = ui->W_fund->text();
    num = ui->W_num->text();
    name = ui->W_name->text();
    reward = ui->W_reward->text();
    postWage = ui->W_postWage->text();
    payedWage = ui->W_payedWage->text();


    float basic = basicWage.toFloat();
    float post = postWage.toFloat();
    float net = netWage.toFloat();
    float payed = payedWage.toFloat();
    float rew = reward.toFloat();
    float fun = fund.toFloat();
    float house = houseReform.toFloat();
    float tax = W_tax(net);

    W_st.W_num = strNumber.toInt();
    strcpy(W_st.W_name,name.toUtf8().data());

    W_st.W_basicWage = basic;

    W_st.W_postWage = post;

    W_st.W_netWage = net;
    W_st.W_payedWage = payed;
    W_st.W_reward = rew;
    W_st.W_fund = fun;

    W_st.W_houseReform = house;
    W_st.W_tax = tax;
    W_st.next = NULL;
    if (W_st.W_code == 0 ){
        QMessageBox::warning(this,"警告","请选择日期！");
    }
    if((W_st.W_num<10000)||(W_st.W_basicWage<=0)||(W_st.W_postWage<=0)||(W_st.W_netWage<=0)||(W_st.W_payedWage<=0)||(W_st.W_reward<=0)||(W_st.W_fund<=0)||(W_st.W_houseReform<=0)){
        QMessageBox::warning(this,"警告","请检查输入格式是否有误");
    }else{
        if (W_st.W_code == 0 ){
            QMessageBox::warning(this,"警告","请选择日期！");
        } else{
            if((basic == 0)||(post == 0)||(net==0)||(payed==0)||(rew==0)||(fun==0)||(house ==0)||(W_st.W_num == 0)){
                QMessageBox::warning(this,"警告","请不要留空");
            } else {
                W_pList = W_inputList(W_pList,&W_st);//填入小结构体
                emit WP_update();
                this->close();
            }
        }
    }






}
