TARGET = smclibrary
TEMPLATE = lib
DEFINES += TESTLIB_LIBRARY

QT -= gui

CONFIG += c++latest

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
SOURCES += \
    src/book.cpp \
    src/library.cpp \
    src/user.cpp

HEADERS += \
    src/book.hpp \
    src/library.hpp \
    src/user.hpp \
    src/message_type.hpp \
    src/rapidcsv.h \

# Directories
DESTDIR=lib                 #Target file directory
OBJECTS_DIR=intermediates   #Intermediate object files directory
MOC_DIR=intermediates       #Intermediate moc files directory

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    books.csv

unix:spellfix.target = spellfix.so
unix:spellfix.commands = gcc -O2 -fPIC -shared src/spellfix.c -o $(DESTDIR)/spellfix.so
win32:sqlite.target = spellfix.dll
win32:spellfix.commands = gcc -O2 -fPIC -shared src/spellfix.c -o $(DESTDIR)/spellfix.dll

all.depends += spellfix

QMAKE_EXTRA_TARGETS += all spellfix
