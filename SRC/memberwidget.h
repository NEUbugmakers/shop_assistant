#ifndef MEMBERWIDGET_H
#define MEMBERWIDGET_H

#include <QWidget>
#include "T_member.h"
#include <QStandardItemModel>

namespace Ui {
class MemberWidget;
}

class MemberWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemberWidget(int M_sum,struct Member* M_head,QWidget *parent = nullptr);
    ~MemberWidget();
    void Mdelete();
    void Mdisplay();
    void Madd();
    void Mselect();
    void Mchoosedisplay();

signals:
    void S_memberadd();
    void S_memberedit();
    void S_membersearchaccurate();
    void S_membersearch(char* number);
    void S_memberchoose();
private slots:
    void M_memberedit(int row,int column);
    void S_searchfuzzy(char* number);
signals:
    void sendsignal();
private:
    Ui::MemberWidget *ui;
    struct Member* mp=nullptr,*Shead = nullptr,*Mhead=nullptr;
    struct Member_choose* Mchoose=nullptr, *mcp = nullptr;
    int sum=0,M_back=0,editmode=0;
    int T_memberchoose[9]={1,1,1,1,1,1,1,-1,99999999};//前五个表示会员等级的筛选 后面两个是到期的筛选
};

#endif // MEMBERWIDGET_H
