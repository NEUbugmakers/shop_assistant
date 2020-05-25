#ifndef SR_REPLENISHREMIND_H
#define SR_REPLENISHREMIND_H

#include <QDialog>
#include "SR_data.h"
namespace Ui {
class sr_replenishremind;
}

class sr_replenishremind : public QDialog
{
    Q_OBJECT

public:
    explicit sr_replenishremind(QWidget *parent = nullptr);
    ~sr_replenishremind();

private:
    void loadInfo();
    Ui::sr_replenishremind *ui;
};

#endif // SR_REPLENISHREMIND_H
