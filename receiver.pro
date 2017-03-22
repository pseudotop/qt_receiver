QT += network
QT += core gui

target.path = /home/pi/
TARGET = receiver
#CONFIG += console
#CONFIG += ordered
#CONFIG -= app_bundle

INCLUDEPATH += /usr/include/qt4

TEMPLATE = app

SOURCES += main.cpp \
          widget.cpp \
          receiver.cpp

HEADERS += widget.h \
           receiver.h

FORMS += widget.ui
