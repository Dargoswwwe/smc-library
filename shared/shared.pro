QT -= gui

TARGET = smclibrary
TEMPLATE = lib
DEFINES += TESTLIB_LIBRARY

CONFIG += c++latest

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
SOURCES += \
    src/book.cpp \
    src/user.cpp \
    src/library.cpp \

HEADERS += \
    src/book.hpp \
    src/user.hpp \
    src/library.hpp \

# Directories
DESTDIR=lib                 #Target file directory
OBJECTS_DIR=intermediates   #Intermediate object files directory
MOC_DIR=intermediates       #Intermediate moc files directory
UI_DIR=intermediates        #Intermediate uic files directory

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
