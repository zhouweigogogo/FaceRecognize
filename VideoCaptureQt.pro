#-------------------------------------------------
#
# Project created by QtCreator 2022-04-06T15:53:51
#
#-------------------------------------------------

QT       += core gui
QT += axcontainer
# 设置C++11
QMAKE_CXXFLAGS += -std=c++11
CONFIG   += C++11
CONFIG   += axcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets core gui
TARGET = Video
TEMPLATE = app
UI_DIR=./

SOURCES += main.cpp\
    face_recognize.cpp \
#    secondwindow.cpp \
    mythread.cpp \
    administrator.cpp \
#    sql.cpp \
#    sqlite3_db.cpp \
#    recognized.cpp \
    firstwindow.cpp \
    thirdwindow.cpp \
    save_csv.cpp
HEADERS  += \
    face_recognize.h \
    /lib/ncnn/ncnn/src/ncnn-20200106/src/net.h \
#    secondwindow.h \
    mythread.h \
    administrator.h \
#    sql.h \
#    sqlite3_db.h \
#    recognized.h \
    firstwindow.h \
    thirdwindow.h \
    save_csv.h
FORMS    += \
#        secondwindow.ui \
#        loadingwidget.ui \
#        sql.ui \
    administrator.ui \
    thirdwindow.ui \
    firstwindow.ui

RESOURCES += load.qrc \
              res.qrc

INCLUDEPATH += $$PWD/lib/opencv/include \
                $$PWD/lib \
                $$PWD/lib/ncnn/ncnn/src/ncnn-20200106/src

DEPENDPATH += $$PWD/lib/opencv/lib \
              $$PWD/lib \
              $$PWD/lib/ncnn/x86

#sylixos {
#model.files = ./data/model
#model.path = /apps/VideoCaptureQt
#faces.files = ./data/faces
#faces.path = /apps/VideoCaptureQt
#INSTALLS += model faces
#}

unix:!macx: LIBS += -L$$PWD/lib/opencv/lib -lopencv_videoio -latomic

INCLUDEPATH += $$PWD/lib/ncnn/x86
DEPENDPATH += $$PWD/lib/ncnn/x86

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ncnn/x86/release/ -lncnn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ncnn/x86/debug/ -lncnn
else:unix: LIBS += -L$$PWD/lib/ncnn/x86/ -lncnn

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lsqlite3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lsqlite3
else:unix: LIBS += -L$$PWD/lib/ -lsqlite3

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
