#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <shop_mainwindow.h>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT
public slots:
    void reshow();
public:
    explicit login(QWidget *parent = nullptr);
    ~login();
signals:
    void sendsignal();
protected:
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器

private slots:
    void on_passport_clicked();
private:
    Ui::login *ui;
    shop_mainwindow *section_mainwindow;
};

#endif // LOGIN_H
