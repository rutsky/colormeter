TEMPLATE = app

CONFIG += warn_on \
          qt \
          thread

DEPENDPATH += src \
              resources

DESTDIR     = bin
OBJECTS_DIR = tmp
MOC_DIR     = tmp
RCC_DIR     = tmp

HEADERS   += imageviewer.h
SOURCES   += main.cpp imageviewer.cpp
RESOURCES += colormeter.qrc

TARGET = colormeter


