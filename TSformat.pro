#-------------------------------------------------
#
# Project created by QtCreator 2016-02-16T01:50:19
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++14
V8DIR = ~/Downloads/v8 #MUST SET THIS TO YOUR v8 DIRECTORY
LIBS     += -Wl,--start-group \
    $${V8DIR}/out/x64.release/obj.target/tools/gyp/libv8_base.a \
    $${V8DIR}/out/x64.release/obj.target/tools/gyp/libv8_libbase.a \
    $${V8DIR}/out/x64.release/obj.target/tools/gyp/libv8_external_snapshot.a \
    $${V8DIR}/out/x64.release/obj.target/tools/gyp/libv8_libplatform.a \
    $${V8DIR}/out/x64.release/obj.target/third_party/icu/libicuuc.a \
    $${V8DIR}/out/x64.release/obj.target/third_party/icu/libicui18n.a \
    $${V8DIR}/out/x64.release/obj.target/third_party/icu/libicudata.a \
    -Wl,--end-group -lrt -ldl -pthread

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TSformat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    format.cpp \
    color.cpp \
    util.cpp \
    formatting.cpp

HEADERS  += mainwindow.h \
    format.h \
    color.h \
    util.h \
    fchar.h \
    formatting.h

FORMS    += mainwindow.ui
