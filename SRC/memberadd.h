#ifndef MEMBERADD_H
#define MEMBERADD_H

#include <QDialog>
#include "memberwidget.h"
#include "tradeadd.h"

namespace Ui {
class memberadd;
}

class memberadd : public QDialog
{
    Q_OBJECT

public:
    explicit memberadd(struct Member* M_head,MemberWidget *Mparent = nullptr,QWidget *parent = nullptr);
    explicit memberadd(char M_num[],char M_sex,struct Member* memberbefore,struct Member* M_head,MemberWidget *Mparent,QWidget *parent);
    ~memberadd();

private:
    Ui::memberadd *ui;
    char M_sex='1';
};

#endif // MEMBERADD_H
