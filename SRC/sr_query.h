#ifndef SR_QUERY_H
#define SR_QUERY_H

#include <QDialog>
#include <QMessageBox>
#include "SR_data.h"
namespace Ui {
class SR_query;
}

class SR_query : public QDialog
{
    Q_OBJECT

public:
    explicit SR_query(QWidget *parent = nullptr,char* tname = NULL);
    ~SR_query();

private:
    void loadInfo();
    char name[C_NAME_LEN];
    Ui::SR_query *ui;
};

#endif // SR_QUERY_H
