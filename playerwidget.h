#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>

#include "playlistitem.h"

namespace Ui {
class playerWidget;
}

class playerWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit playerWidget(QWidget *parent = 0);
    ~playerWidget();

    void play(PlaylistItem *item);
    void fillFields(PlaylistItem *item);
    QString time(int time);
    
private:
    Ui::playerWidget *ui;
};

#endif // PLAYERWIDGET_H
