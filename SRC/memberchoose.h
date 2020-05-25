#ifndef MEMBERCHOOSE_H
#define MEMBERCHOOSE_H

#include <QDialog>
#include "memberwidget.h"
namespace Ui {
class memberchoose;
}

class memberchoose : public QDialog
{
    Q_OBJECT

public:
    explicit memberchoose(int memberchoose[],struct Member* M_head,MemberWidget *Mparent = nullptr,QWidget *parent = nullptr);
    ~memberchoose();

private:
    Ui::memberchoose *ui;
};

#endif // MEMBERCHOOSE_H
