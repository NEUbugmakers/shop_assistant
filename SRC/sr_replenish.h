#ifndef SR_REPLENISH_H
#define SR_REPLENISH_H

#include <QDialog>
#include <QMessageBox>
#include "SR_data.h"
namespace Ui {
class sr_replenish;
}

class sr_replenish : public QDialog
{
    Q_OBJECT

public:
    explicit sr_replenish(QWidget *parent = nullptr);
    ~sr_replenish();

private:
    void query();
    void add();
    Ui::sr_replenish *ui;
};

#endif // SR_REPLENISH_H
