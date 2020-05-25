#ifndef C_GOODSINFOSHOW_H
#define C_GOODSINFOSHOW_H

#include <QDialog>
#include <QTreeWidget>
#include <QDebug>
#include <QMessageBox>
#include "c_batchesshow.h"
#include "c_salesshow.h"
#include "SR_data.h"
namespace Ui {
class c_goodsinfoshow;
}

class c_goodsinfoshow : public QDialog
{
    Q_OBJECT

public:
    explicit c_goodsinfoshow(QWidget *parent = nullptr,char* tcode=NULL);
    void remove();
    void save();
    void shelf();
    void stock();
    void sales();
    ~c_goodsinfoshow();

private:
    void loadInfo();
    char code[C_CODE_LEN];
    Ui::c_goodsinfoshow *ui;
};

#endif // C_GOODSINFOSHOW_H
