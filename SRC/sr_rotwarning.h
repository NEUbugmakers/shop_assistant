#ifndef SR_ROTWARNING_H
#define SR_ROTWARNING_H

#include <QDialog>
#include <QMessageBox>
#include "SR_data.h"
namespace Ui {
class sr_rotwarning;
}

class sr_rotwarning : public QDialog
{
    Q_OBJECT

public:
    explicit sr_rotwarning(QWidget *parent = nullptr);
    ~sr_rotwarning();
private:
    void loadInfo();
    void toDo(int row,int column);
    Ui::sr_rotwarning *ui;
};

#endif // SR_ROTWARNING_H
