#ifndef SR_ADDNEWSORT_H
#define SR_ADDNEWSORT_H

#include <QDialog>
#include <QMessageBox>
#include "SR_data.h"
namespace Ui {
class sr_addnewsort;
}

class sr_addnewsort : public QDialog
{
    Q_OBJECT

public:
    explicit sr_addnewsort(QWidget *parent = nullptr,char* tsort=NULL);
    ~sr_addnewsort();

private:
    Ui::sr_addnewsort *ui;
    char sort[C_SORT_LEN];
    void addNewSort();
};

#endif // SR_ADDNEWSORT_H
