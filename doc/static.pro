unix:!macx: LIBS += -L$$PWD/../../Qt5.7.0/5.7/Src/qtbase/lib/ -lQt5Widgets

INCLUDEPATH += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/include
DEPENDPATH += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/lib/libQt5Widgets.a

unix:!macx: LIBS += -L$$PWD/../../Qt5.7.0/5.7/Src/qtbase/lib/ -lQt5Gui

INCLUDEPATH += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/include
DEPENDPATH += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/lib/libQt5Gui.a

unix:!macx: LIBS += -L$$PWD/../../Qt5.7.0/5.7/Src/qtbase/lib/ -lQt5Network

INCLUDEPATH += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/include
DEPENDPATH += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/lib/libQt5Network.a

unix:!macx: LIBS += -L$$PWD/../../Qt5.7.0/5.7/Src/qtbase/lib/ -lQt5Sql

INCLUDEPATH += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/include
DEPENDPATH += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/lib/libQt5Sql.a

unix:!macx: LIBS += -L$$PWD/../../Qt5.7.0/5.7/Src/qtbase/lib/ -lQt5Core

INCLUDEPATH += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/include
DEPENDPATH += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../Qt5.7.0/5.7/Src/qtbase/lib/libQt5Core.a

