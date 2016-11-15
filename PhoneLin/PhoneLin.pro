#-------------------------------------------------
#
# Project created by QtCreator 2015-12-31T10:57:59
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhoneLin
TEMPLATE = app


SOURCES += main.cpp \
    controler/appProxy.cpp \
    controler/deviceAgent.cpp \
    controler/deviceMonitor.cpp \
    controler/jsonHelper.cpp \
    controler/pimProxy.cpp \
    controler/storageProxy.cpp \
    db/dbHelper.cpp \
    unit_test/testDb.cpp \
    unit_test/testMonitor.cpp \
    view/appview.cpp \
    view/calllogModel.cpp \
    view/calllogview.cpp \
    view/contactHtmlDelegate.cpp \
    view/contactTableModel.cpp \
    view/contactview.cpp \
    view/deviceview.cpp \
    view/fileview.cpp \
    view/mainwindow.cpp \
    view/smschatview.cpp \
    view/smseditorview.cpp \
    view/smsmodel.cpp \
    view/smsview.cpp \
    3rdpart/PageWidget.cpp \
    view/appHtmlDelegate.cpp \
    view/appTableModel.cpp \
    view/fileHtmlDelegate.cpp \
    view/fileTableModel.cpp \
    view/mimeTypes.cpp \
    view/input_dialog.cpp \
    view/plsetting.cpp \
    controler/monitorThread.cpp



        

HEADERS  += \
    android_const.h \
    phonelin.h \
    controler/appProxy.h \
    controler/deviceAgent.h \
    controler/deviceMonitor.h \
    controler/jsonHelper.h \
    controler/netPackage.h \
    controler/pimProxy.h \
    controler/storageProxy.h \
    db/dbHelper.h \
    unit_test/testDb.h \
    unit_test/testMonitor.h \
    view/appview.h \
    view/calllogModel.h \
    view/calllogview.h \
    view/contactHtmlDelegate.h \
    view/contactTableModel.h \
    view/contactview.h \
    view/deviceview.h \
    view/fileview.h \
    view/mainwindow.h \
    view/smschatview.h \
    view/smseditorview.h \
    view/smsmodel.h \
    view/smsview.h \
    3rdpart/PageWidget.h \
    view/appHtmlDelegate.h \
    view/appTableModel.h \
    view/fileHtmlDelegate.h \
    view/fileTableModel.h \
    view/mimeTypes.h \
    view/input_dialog.h \
    view/plsetting.h \
    controler/monitorThread.h



FORMS    += \
    ui/app.ui \
    ui/calllog.ui \
    ui/contact.ui \
    ui/device.ui \
    ui/file.ui \
    ui/mainwindow.ui \
    ui/sms.ui \
    ui/smschat.ui \
    ui/smseditor.ui \
    3rdpart/PageWidget.ui \
    ui/plsetting.ui

OTHER_FILES +=

RESOURCES += res/phonelin_res.qrc

QMAKE_CXXFLAGS += -std=c++11  -std=gnu++11

windows {
   LIBS += -lwsock32
}



