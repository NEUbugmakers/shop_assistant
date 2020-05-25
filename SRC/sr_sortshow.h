#ifndef SR_SORTSHOW_H
#define SR_SORTSHOW_H

#include <QDialog>
#include <QMessageBox>
#include "sr_addnewgoods.h"
#include "sr_addnewsort.h"
#include "SR_data.h"
namespace Ui {
class sr_sortshow;
}

class sr_sortshow : public QDialog
{
    Q_OBJECT

public:
    explicit sr_sortshow(QWidget *parent = nullptr,char* tcode=NULL);
    ~sr_sortshow();

private:
    void addNewGoods();
    void addNewSort();
    void remove();
    char code[C_SORT_LEN];
    void loadInfo();
    void save();
    Ui::sr_sortshow *ui;
};

#endif // SR_SORTSHOW_H
