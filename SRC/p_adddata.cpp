#include "p_adddata.h"
#include "ui_p_adddata.h"

P_addData::P_addData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::P_addData)
{
    ui->setupUi(this);
    this->setWindowTitle("添加人事信息");
    //默认选男
    ui->ra_Man->setChecked(true);
    this->P_isMan =true;
    connect(ui->ra_Woman,&QPushButton::clicked,[=](){
       this->P_isMan =false;
    });
    connect(ui->ra_Man,&QPushButton::clicked,[=](){
        this->P_isMan =true;
    });




    ui->P_name->setPlaceholderText("请输入不超过四个字的中文或英文");
    ui->P_name->setMaxLength(4);
    ui->P_name->setClearButtonEnabled(true);


    ui->P_num->setPlaceholderText("10000~99999");
    ui->P_num->setMaxLength(5);
    ui->P_num->setClearButtonEnabled(true);


    ui->P_post->setPlaceholderText("不超过5个汉字");
    ui->P_post->setMaxLength(5);
    ui->P_post->setClearButtonEnabled(true);

    ui->P_phone->setPlaceholderText("11位手机号");
    ui->P_phone->setMaxLength(11);
    //最小值检验
    ui->P_phone->setClearButtonEnabled(true);

    ui->P_highestEducation->setPlaceholderText("学历：本科，高中等");
    ui->P_highestEducation->setMaxLength(2);
    ui->P_highestEducation->setClearButtonEnabled(true);

    ui->P_address->setPlaceholderText("住址：不超过12个汉字");
    ui->P_address->setMaxLength(12);
    ui->P_address->setClearButtonEnabled(true);





    P_initial(&P_st);//初始化接受结构体

    ui->P_entry->setDateTime(QDateTime::currentDateTime());

    ui->P_entry->setDisplayFormat("yyyy.M.d");
    ui->P_entry->setMaximumDate(QDate::currentDate().addDays(0));
    ui->P_entry->setCalendarPopup(true);
    connect(ui->P_entry, SIGNAL(dateChanged(QDate)), this, SLOT(onDateChanged1(QDate)));


    ui->P_birthday->setDateTime(QDateTime::currentDateTime());

    ui->P_birthday->setDisplayFormat("yyyy.M.d");
    ui->P_birthday->setMaximumDate(QDate::currentDate().addDays(0));
    ui->P_birthday->setCalendarPopup(true);
    connect(ui->P_birthday, SIGNAL(dateChanged(QDate)), this, SLOT(onDateChanged2(QDate)));


//    connect(ui->P_save,&QPushButton::clicked,[=](){
//        emit WP_update();
//        this->close();
//    });



}

P_addData::~P_addData()
{
    delete ui;
}

void P_addData::onDateChanged1(const QDate &date){//入职日期
    int year,mon;
    QString strYear = ui->P_entry->sectionText(QDateTimeEdit::YearSection);
    QString strMonth = ui->P_entry->sectionText(QDateTimeEdit::MonthSection);
    QString strDay = ui->P_entry->sectionText(QDateTimeEdit::DaySection);
    year = strYear.toInt();
    mon = strMonth.toInt();
    P_st.P_entrydate.B_Year = year;
    P_st.P_entrydate.B_Months = mon;

}


void P_addData::onDateChanged2(const QDate &date)//出生日期
{
    int year,mon;
    QString strYear = ui->P_birthday->sectionText(QDateTimeEdit::YearSection);
    QString strMonth = ui->P_birthday->sectionText(QDateTimeEdit::MonthSection);
    QString strDay = ui->P_birthday->sectionText(QDateTimeEdit::DaySection);
    year = strYear.toInt();
    mon = strMonth.toInt();
    P_st.P_birthday.B_Year = year;
    P_st.P_birthday.B_Months = mon;
}
void P_addData::on_P_preview_clicked()
{
    this->close();
}

void P_addData::on_P_save_clicked()
{
    QString strname,strhighestEducation,strnum,strpost,strphone,straddress;
    //姓名
    strname = ui->P_name->text();
    strcpy(P_st.P_name,strname.toUtf8().data());
    //学历
    strhighestEducation = ui->P_highestEducation->text();
    strcpy(P_st.P_highestEducation,strhighestEducation.toUtf8().data());
    //工号
    strnum = ui->P_num->text();
    int num = strnum.toUInt();
    P_st.P_num = num;
    //岗位
    strpost = ui->P_post->text();
    strcpy(P_st.P_post,strpost.toUtf8().data());
    //手机
    strphone = ui->P_phone->text();
    strcpy(P_st.P_phone,strphone.toUtf8().data());
    //地址
    straddress = ui->P_address->text();
    strcpy(P_st.P_address,straddress.toUtf8().data());
    //性别
    if (this->P_isMan == true){
       strcpy(P_st.P_sex,"男");
    } else{
        strcpy(P_st.P_sex,"女");
    }

    if((P_st.P_num<10000)||(!Isnumber(P_st.P_phone))||(P_st.P_phone[0]!='1')||(strlen(P_st.P_phone)<11)){
        QMessageBox::warning(this,"警告","请检查输入格式是否有误");
    }else{
        if((P_st.P_birthday.B_Year == 0)||(P_st.P_entrydate.B_Year == 0)){
            QMessageBox::warning(this,"警告","请选择日期！");
        } else
        {
            if((strname.isEmpty())||(strhighestEducation.isEmpty())||(strnum.isEmpty())||(strpost.isEmpty())||(strphone.isEmpty())||(straddress.isEmpty())){
                QMessageBox::warning(this,"警告","请不要留空");
            } else{
                P_pList = P_inputList(P_pList,&P_st);
                qDebug()<<"test";
                emit WP_update();
                this->close();
            }
        }

    }





}





            int Isnumber(char * str){//判断是否全为数字
                int flag = 1;
                for( int i = 0; i < strlen(str); i++ )
                {
                    if( str[i] >= '0' && str[i] <='9' )
                        continue;
                    else
                    {
                        flag = 0;
                        break;
                    }
                }
                if( flag == 0 ){
                    return 0;
                }else{
                    return 1;
                }
            }












