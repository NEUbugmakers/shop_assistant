#ifndef P_ADDDATA_H
#define P_ADDDATA_H

#include <QDialog>
#include <QDebug>
#include <WP_basic.h>
#include <WP_file.h>
#include <QMessageBox>
namespace Ui {
class P_addData;
}

class P_addData : public QDialog
{
    Q_OBJECT

public:
    explicit P_addData(QWidget *parent = nullptr);
    ~P_addData();
    bool P_isMan;

private slots:
    void onDateChanged1(const QDate &date);
    void onDateChanged2(const QDate &date);


    void on_P_preview_clicked();

    void on_P_save_clicked();

signals:
    void WP_update();

private:
    Ui::P_addData *ui;

};
int Isnumber(char * str);
#endif // P_ADDDATA_H
