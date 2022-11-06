QT += core gui
QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17 console
CONFIG += app_bundle
CONFIG += c++17
CONFIG += precompile_header
PRECOMPILED_HEADER = pre_compiled.hpp
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.# You can make your code fail to compile if it uses deprecated APIs.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin# Default rules for deployment.
else: unix:!android: target.path = /opt/$${TARGET}/binqnx: target.path = /tmp/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += targetelse: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    crc.hpp \
    pre_compiled.hpp \
    structs.hpp \
    toZipData.hpp \
    tounzipdata.hpp \
    filemetadata.hpp \
    pre_compiled.hpp

QT += widgets quick

SOURCES += \
  crc.cpp \
  filemetadata.cpp \
  main.cpp \
  structs.cpp \
  toZipData.cpp \
  tounzipdata.cpp \
  userinterface.cpp \
  zipcontroller.cpp
