#ifndef MYITEM_H
#define MYITEM_H

#include <QtCore>
#include <QString>

class PlaylistItem
{
public:
    QString artist;
    QString song;
    QString time;
    PlaylistItem();
};
Q_DECLARE_METATYPE(PlaylistItem *)
#endif // MYITEM_H
