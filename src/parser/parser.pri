include (flex.pri)
include (bison.pri)

HEADERS += \
  parser/yyheader.h \
  parser/parser.h \
  parser/structuredtextparser.h \
  parser/featuresparser.h

SOURCES += \
  parser/parser.cpp \
  parser/structuredtextparser.cpp \
  parser/featuresparser.cpp

FLEXSOURCES += parser/db.l
BISONSOURCES += parser/db.y