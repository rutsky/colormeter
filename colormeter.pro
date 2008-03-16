TEMPLATE = app

CONFIG += warn_on \
          qt \
          thread \
          resources

DESTDIR     = bin

OBJECTS_DIR = tmp
MOC_DIR     = tmp
RCC_DIR     = tmp
# UI_DIR      = tmp

HEADERS   += src/imageviewer.h \
             src/renderarea.h \
             src/colorstat.h
SOURCES   += src/main.cpp \
             src/imageviewer.cpp \
             src/renderarea.cpp \
             src/colorstat.cpp
RESOURCES += resources/colormeter.qrc

macx {
  TARGET = ColorMeter
}
unix {
  TARGET = colormeter
  CONFIG += debug   # debug
}
win32 {
  TARGET = ColorMeter
  CONFIG -= debug_and_release
}
