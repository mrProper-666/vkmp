QT       += core gui phonon network

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
    vk.cpp

HEADERS  += roster.h \
    playerwidget.h \
    playlistitem.h \
    playlistitemwidget.h \
    playlistmodel.h \
    playlistitemdelegate.h \
    vk.h

FORMS    += roster.ui \
    playerwidget.ui

RESOURCES += \
    resources.qrc
