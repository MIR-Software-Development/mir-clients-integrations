QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CreatePatientSessionWindow.cpp \
    CreatePatientWindow.cpp \
    HL7.cpp \
    Patient.cpp \
    WXP.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    CreatePatientSessionWindow.h \
    CreatePatientWindow.h \
    HL7.h \
    MainWindow.h \
    Patient.h \
    WXP.h

FORMS += \
    CreatePatientSessionWindow.ui \
    CreatePatientWindow.ui \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
