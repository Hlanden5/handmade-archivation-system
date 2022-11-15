QT += core gui
QT += core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
PRECOMPILED_HEADER = pre_compiled.hpp
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    crc.cpp \
    filemetadata.cpp \
    main.cpp \
    structs.cpp \
    toZipData.cpp \
    tounzipdata.cpp \
    userinterface.cpp \
    zipcontroller.cpp

HEADERS += \
    crc.hpp \
    filemetadata.hpp \
    pre_compiled.hpp \
    structs.hpp \
    toZipData.hpp \
    tounzipdata.hpp \
    userinterface.hpp \
    zipcontroller.hpp

FORMS += \
    mainwindow.ui \
    userinterface.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
