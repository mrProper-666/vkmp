#ifndef PLAYLISTITEMDELEGATE_H
#define PLAYLISTITEMDELEGATE_H

#include <QItemDelegate>
#include "QPainter"

#include "playlistitemwidget.h"
#include "playlistitem.h"

class PlaylistItemDelegate : public QItemDelegate
{
public:
    PlaylistItemDelegate();
    PlaylistItemWidget *wdg;
    /*Метод для отрисовки нашего виджета*/
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    /*Возвращает нужный размер нашего виджета*/
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // PLAYLISTITEMDELEGATE_H
