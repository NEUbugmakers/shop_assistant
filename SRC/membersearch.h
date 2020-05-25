#ifndef MEMBERSEARCH_H
#define MEMBERSEARCH_H
#include "memberwidget.h"
#include "tradewidget.h"
#include <QDialog>

namespace Ui {
class membersearch;
}

class membersearch : public QDialog
{
    Q_OBJECT

public:
    explicit membersearch(struct Member** S_head,struct Member* M_head,MemberWidget *Mparent = nullptr,QWidget *parent = nullptr);
    explicit membersearch(struct Member** S_head,struct Member* M_head,tradewidget *Tparent = nullptr,QWidget *parent = nullptr);
    ~membersearch();

private:
    Ui::membersearch *ui;
};


#endif // MEMBERSEARCH_H
