#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>

#include "playlistitem.h"

class PlaylistModel : public QAbstractListModel
{
public:
    explicit PlaylistModel(QObject *parent = 0);
    QList<PlaylistItem *> list;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // PLAYLISTMODEL_H
