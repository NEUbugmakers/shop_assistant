#ifndef C_SALESSHOW_H
#define C_SALESSHOW_H

#include <QDialog>
#include "SR_data.h"
namespace Ui {
class c_salesshow;
}

class c_salesshow : public QDialog
{
    Q_OBJECT

public:
    explicit c_salesshow(QWidget *parent = nullptr,char* tcode = NULL);
    ~c_salesshow();

private:
    char code[C_CODE_LEN];
    void loadInfo();
    Ui::c_salesshow *ui;
};

#endif // C_SALESSHOW_H
