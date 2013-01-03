#include "playlistitemwidget.h"

PlaylistItemWidget::PlaylistItemWidget(QWidget *parent) :
    QWidget(parent)
{
    artist = new QLabel;
    song = new QLabel;
    time = new QLabel;
    vertLay = new QVBoxLayout;
    QFont font;
    font.setBold(true);
    artist->setFont(font);
    artist->setMaximumWidth(270);
    song->setMaximumWidth(260);
    vertLay->addWidget(artist);
    vertLay->addWidget(song);
    vertLay->setContentsMargins(0,0,0,0);
    vertLay->setSpacing(0);
    hLay = new QHBoxLayout;
    hLay->addLayout(vertLay);
    hLay->addStretch(1);
    hLay->addWidget(time);
    hLay->setContentsMargins(15,5,15,5);
    hLay->setSpacing(0);
    this->setLayout(hLay);
}
