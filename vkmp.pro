QT       += core gui phonon network xml webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vkMP
TEMPLATE = app

UI_DIR      = temp_ui
MOC_DIR     = temp_moc
OBJECTS_DIR = temp_o

DESTDIR     = bin


SOURCES += main.cpp\
        roster.cpp \
    playerwidget.cpp \
    playlistitem.cpp \
    playlistitemwidget.cpp \
    playlistmodel.cpp \
    playlistitemdelegate.cpp \
    vk.cpp \
    settings.cpp

HEADERS  += roster.h \
    playerwidget.h \
    playlistitem.h \
    playlistitemwidget.h \
    playlistmodel.h \
    playlistitemdelegate.h \
    vk.h \
    structs.h \
    settings.h

FORMS    += roster.ui \
    playerwidget.ui

RESOURCES += \
    resources.qrc

win32:RC_FILE = winRCFile.rc
