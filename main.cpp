#include <QApplication>
#include <QTextCodec>

#include "roster.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);

    a.setApplicationName(QObject::tr("vkMp"));
    a.setApplicationVersion(QObject::tr("v0.1"));
    a.setObjectName(QObject::tr("PortveinSoft"));

    Roster w;
    w.show();
    
    return a.exec();
}
