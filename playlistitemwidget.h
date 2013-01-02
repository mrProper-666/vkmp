#ifndef PLAYLISTITEMWIDGET_H
#define PLAYLISTITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLayout>

class PlaylistItemWidget : public QWidget
{
    Q_OBJECT
public:
    QLabel *artist;
    QLabel *song;
    QLabel *time;
    QVBoxLayout *vertLay;
    QHBoxLayout *hLay;
    explicit PlaylistItemWidget(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // PLAYLISTITEMWIDGET_H
