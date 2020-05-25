#ifndef MEMBERGOOD_H
#define MEMBERGOOD_H

#include <QDialog>

namespace Ui {
class MemberGood;
}

class MemberGood : public QDialog
{
    Q_OBJECT

public:
    explicit MemberGood(struct Member* Mp,QWidget *parent = nullptr);
    ~MemberGood();
    void T_recordsearch(int M_number, FILE* fp);
    void M_recordsearch(int M_number, struct M_trade* M_head);
    void M_memberdisplay();
private slots:
    void M_Gooddisplay(int row,int column);
private:
    Ui::MemberGood *ui;
    int sum=0,M_back=0;
    struct Member* Mp;
    struct M_trade* mtp;
};

#endif // MEMBERGOOD_H
