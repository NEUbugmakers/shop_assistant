#include "login.h"
#include "ui_login.h"
#include "A_systerm.h"
#include "QtDebug"
login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    connect(ui->pushButton_exit,&QPushButton::clicked,[=](){
        qDebug()<<"exit";
        this->close();
    });
//    shop_mainwindow mainwidget(nullptr);
//    mainwidget.show();
//    mainwidget.setWindowTitle("超市小助手");
    section_mainwindow = new shop_mainwindow;
    section_mainwindow->setWindowTitle("超市小助手");
    ui->lineEdit_passport->setEchoMode(QLineEdit::Password);
    ui->pushButton_login->setFocus();
    ui->pushButton_login->setDefault(true);
    section_mainwindow->resize(340,440);
    connect(section_mainwindow,SIGNAL(sendsignal()),this,SLOT(reshow()));
    connect(ui->pushButton_login,&QPushButton::clicked,this,&login::on_passport_clicked);
    ui->lineEdit_passport->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_passport->setFocus();
    ui->lineEdit_passport->installEventFilter(this);//设置完后自动调用其eventFilter函数
}

login::~login()
{
    delete ui;
}
void login::reshow(){
    this->show();
}
bool login::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->lineEdit_passport)
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 on_passport_clicked();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}

void login::on_passport_clicked()
{
    A_account user;
    strcpy(user.A_password,ui->lineEdit_passport->text().toUtf8().data());
    user.A_num=ui->lineEdit_account->text().toInt();
    qDebug()<<user.A_password;
    qDebug()<<user.A_num;
    if(A_signUp(&user,A_readFile())){
        section_mainwindow->show();
        this->hide();
        ui->lineEdit_passport->setText("");
        ui->lineEdit_account->setText("");
    }else QMessageBox::warning(this,"警告","输入的账号密码不匹配！");
}
