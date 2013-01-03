#ifndef MYITEM_H
#define MYITEM_H

#include <QtCore>
#include <QString>
#include <QUrl>

class PlaylistItem
{
public:
    QString artist;
    QString song;
    QString time;
    QString id;
    QString aid;
    QUrl url;
    PlaylistItem();
};
Q_DECLARE_METATYPE(PlaylistItem *)
#endif // MYITEM_H
