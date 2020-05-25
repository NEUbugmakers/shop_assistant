#ifndef C_GETOUTAMOUNT_H
#define C_GETOUTAMOUNT_H

#include <QDialog>
#include <QMessageBox>
#include "SR_data.h"
namespace Ui {
class c_getoutamount;
}

class c_getoutamount : public QDialog
{
    Q_OBJECT

public:
    explicit c_getoutamount(QWidget *parent = nullptr,char* tcode=NULL, char tbatch=0);
    ~c_getoutamount();

private:
    char code[C_CODE_LEN];
    char batch;
    void outofstock();
    Ui::c_getoutamount *ui;
};

#endif // C_GETOUTAMOUNT_H
