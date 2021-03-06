TEMPLATE = app

MOC_DIR = .build
UI_DIR = .build
RCC_DIR = .build
OBJECTS_DIR = .build
BUILD_DIR = .build

include (parser/parser.pri)
include (symbol/symbol.pri)
include (tests/tests.pri)

HEADERS += \
  archiveloader.h \
  context.h \
  feature.h \
  graphicslayer.h \
  graphicslayerscene.h \
  layer.h \
  notes.h \
  odbppgraphicsscene.h \
  odbppgraphicsview.h \
  profile.h \
  settings.h \
  symbolfactory.h

SOURCES += \
  archiveloader.cpp \
  context.cpp \
  feature.cpp \
  layer.cpp \
  graphicslayer.cpp \
  graphicslayerscene.cpp \
  notes.cpp \
  odbppgraphicsscene.cpp \
  odbppgraphicsview.cpp \
  profile.cpp \
  settings.cpp

INCLUDEPATH += . .build parser parser/record symbol

DESTDIR = ../bin
TARGET = test
