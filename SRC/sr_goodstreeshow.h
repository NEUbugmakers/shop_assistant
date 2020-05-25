#ifndef SR_GOODSTREESHOW_H
#define SR_GOODSTREESHOW_H

#include <QWidget>
#include <QTreeWidget>
#include <QDebug>
#include <QTreeWidget>
#include <ctype.h>
#include <QMenu>
#include "c_goodsinfoshow.h"
#include "c_outofstock.h"
#include "sr_sortshow.h"
#include "sr_replenish.h"
#include "sr_addnewsort.h"
#include "sr_rotwarning.h"
#include "sr_replenishremind.h"
#include "sr_query.h"
#include "SR_data.h"
namespace Ui {
class sr_goodstreeshow;
}

class sr_goodstreeshow : public QWidget
{
    Q_OBJECT

public:
    explicit sr_goodstreeshow(QWidget *parent = nullptr);
    void update();
    void remove();
    ~sr_goodstreeshow();
public slots:
        void showInfo(QTreeWidgetItem *item, int column);
        void replenish();
        void outofstock();
        void addmainsort();
        void rot();
        void replenishremind();
        void query();
signals:
        void sendsignal();

private:
    Ui::sr_goodstreeshow *ui;
};

#endif // SR_GOODSTREESHOW_H
