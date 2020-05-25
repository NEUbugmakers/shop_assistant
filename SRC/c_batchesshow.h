#ifndef C_BATCHESSHOW_H
#define C_BATCHESSHOW_H

#include <QDialog>
#include <string.h>
#include "SR_data.h"
namespace Ui {
class c_batchesshow;
}

class c_batchesshow : public QDialog
{
    Q_OBJECT

public:
    explicit c_batchesshow(QWidget *parent = nullptr,char* tcode=NULL,int tflag = 0);
    ~c_batchesshow();

private:
    void update();
    Ui::c_batchesshow *ui;
    char code[C_CODE_LEN];
    int flag;
};

#endif // C_BATCHESSHOW_H
