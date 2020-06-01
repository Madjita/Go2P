QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG -= qt

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        alien/CompileErrors.cpp \
        alien/parser.cpp \
        alien/scanner.cpp \
        alien/tlt.cpp \
        my/funccall.cpp \
        my/functype.cpp \
        my/hardtype.cpp \
        main.cpp \
        my/ifelse.cpp \
        my/intermediate.cpp \
        my/myvariable.cpp \
        my/recordhardtype.cpp \
        my/table.cpp \
        my/variableexeption.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    alien/CompileErrors.h \
    alien/Exception.h \
    alien/parser.h \
    alien/scanner.h \
    alien/tlt.h \
    hardtype.h \
    my/funccall.h \
    my/functype.h \
    my/hardtype.h \
    my/ifelse.h \
    my/intermediate.h \
    my/myvariable.h \
    my/recordhardtype.h \
    my/table.h \
    my/variableexeption.h
