#ifndef C_OUTOFSTOCK_H
#define C_OUTOFSTOCK_H

#include <QDialog>
#include "SR_data.h"
#include "c_getoutamount.h"
namespace Ui {
class c_outofstock;
}

class c_outofstock : public QDialog
{
    Q_OBJECT

public:
    explicit c_outofstock(QWidget *parent = nullptr);
    ~c_outofstock();

private:
    char code[C_CODE_LEN];
    void query();
    void update();
    void outofstock(int row,int column);
    Ui::c_outofstock *ui;
};

#endif // C_OUTOFSTOCK_H
