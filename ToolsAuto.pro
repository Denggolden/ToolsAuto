QT       += core gui xml
# qBreakpad中需要使用到network模块
QT += network
# qt线程
QT += concurrent

# 启用多线程、异常、RTTI、STL支持
CONFIG += thread exceptions rtti stl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#本软件
include($$PWD/Src/Common/Common.pri)
include($$PWD/Src/StyleCtrl/StyleCtrl.pri)
include($$PWD/Src/FileHandel/FileHandel.pri)
include($$PWD/Src/DataManage/DataManage.pri)
include($$PWD/Src/TsHandelWin/TsHandelWin.pri)
include($$PWD/Src/Common/Common.pri)
include($$PWD/Src/FileConvertWin/FileConvertWin.pri)
include($$PWD/Src/DataModelEditWin/DataModelEditWin.pri)
include($$PWD/Src/FileCompareDiffWin/FileCompareDiffWin.pri)

#第三方库
include($$PWD/ThirdLib/ThirdLib.pri)

SOURCES += \
    main.cpp \
    MainWin.cpp

HEADERS += \
    MainWin.h

FORMS += \
    MainWin.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#INCLUDEPATH += $$PWD/ThirdLib/qBreakpad/inc
#DEPENDPATH += $$PWD/ThirdLib/qBreakpad/inc

#win32: LIBS += -L$$PWD/ThirdLib/qBreakpad/Lib/debugLib/ -lqBreakpadd
#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/ThirdLib/qBreakpad/Lib/debugLib/qBreakpadd.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/ThirdLib/qBreakpad/Lib/debugLib/libqBreakpadd.a

#win32:CONFIG(release, debug|release): {
#win32: LIBS += -L$$PWD/ThirdLib/qBreakpad/Lib/releaseLib/ -lqBreakpad
#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/ThirdLib/qBreakpad/Lib/releaseLib/qBreakpad.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/ThirdLib/qBreakpad/Lib/releaseLib/libqBreakpad.a
#}
#else:win32:CONFIG(debug, debug|release): {
#win32: LIBS += -L$$PWD/ThirdLib/qBreakpad/Lib/debugLib/ -lqBreakpadd
#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/ThirdLib/qBreakpad/Lib/debugLib/qBreakpadd.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/ThirdLib/qBreakpad/Lib/debugLib/libqBreakpadd.a
#}
