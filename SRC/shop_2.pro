#-------------------------------------------------
#
# Project created by QtCreator 2020-05-23T16:09:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = shop_assistant
TEMPLATE = app
RC_FILE = logo.rc
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    A_systerm.cpp \
    b_backup.cpp \
    B_list.cpp \
    B_time.cpp \
    B_vector.cpp \
    c_batchesshow.cpp \
    c_getoutamount.cpp \
    C_goods.cpp \
    c_goodsinfoshow.cpp \
    C_goodsvector.cpp \
    c_outofstock.cpp \
    c_salesshow.cpp \
    discountwidget.cpp \
    memberadd.cpp \
    memberchoose.cpp \
    membergood.cpp \
    membersearch.cpp \
    memberwidget.cpp \
    p_adddata.cpp \
    shop_mainwindow.cpp \
    sr_addnewgoods.cpp \
    sr_addnewsort.cpp \
    SR_data.cpp \
    sr_goodstreeshow.cpp \
    sr_query.cpp \
    sr_replenish.cpp \
    sr_replenishremind.cpp \
    sr_rotwarning.cpp \
    sr_sortshow.cpp \
    T_member.cpp \
    tradeadd.cpp \
    tradeflowwidget.cpp \
    tradewidget.cpp \
    w_adddata.cpp \
    WP_basic.cpp \
    WP_file.cpp \
    wp_lookup.cpp \
    wp_show.cpp \
    login.cpp

HEADERS += \
    A_systerm.h \
    b_backup.h \
    B_list.h \
    B_time.h \
    B_vector.h \
    c_batchesshow.h \
    c_getoutamount.h \
    C_goods.h \
    c_goodsinfoshow.h \
    C_goodsvector.h \
    c_outofstock.h \
    c_salesshow.h \
    discountwidget.h \
    memberadd.h \
    memberchoose.h \
    membergood.h \
    membersearch.h \
    memberwidget.h \
    p_adddata.h \
    shop_mainwindow.h \
    sr_addnewgoods.h \
    sr_addnewsort.h \
    SR_data.h \
    sr_goodstreeshow.h \
    sr_query.h \
    sr_replenish.h \
    sr_replenishremind.h \
    sr_rotwarning.h \
    sr_sortshow.h \
    T_member.h \
    tradeadd.h \
    tradeflowwidget.h \
    tradewidget.h \
    w_adddata.h \
    WP_basic.h \
    WP_file.h \
    wp_lookup.h \
    wp_show.h \
    login.h

FORMS += \
    c_batchesshow.ui \
    c_getoutamount.ui \
    c_goodsinfoshow.ui \
    c_outofstock.ui \
    c_salesshow.ui \
    dialog.ui \
    memberadd.ui \
    memberchoose.ui \
    membergood.ui \
    membersearch.ui \
    memberwidget.ui \
    p_adddata.ui \
    shop_mainwindow.ui \
    sr_addnewgoods.ui \
    sr_addnewsort.ui \
    sr_goodstreeshow.ui \
    sr_query.ui \
    sr_replenish.ui \
    sr_replenishremind.ui \
    sr_rotwarning.ui \
    sr_sortshow.ui \
    tradeadd.ui \
    tradeflowwidget.ui \
    tradewidget.ui \
    w_adddata.ui \
    wp_lookup.ui \
    wp_show.ui \
    login.ui \
    discountwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/shopicon.qrc
