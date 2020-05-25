#ifndef SR_ADDNEWGOODS_H
#define SR_ADDNEWGOODS_H

#include <QDialog>
#include <QMessageBox>
#include "SR_data.h"
namespace Ui {
class sr_addnewgoods;
}

class sr_addnewgoods : public QDialog
{
    Q_OBJECT

public:
    explicit sr_addnewgoods(QWidget *parent = nullptr,char*sort=NULL);
    ~sr_addnewgoods();

private:
    char sort[C_SORT_LEN];
    void add();
    Ui::sr_addnewgoods *ui;
};

#endif // SR_ADDNEWGOODS_H
