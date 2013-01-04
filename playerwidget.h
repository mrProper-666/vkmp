#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include <Phonon/Phonon>

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
    void saveSelected(PlaylistItem *item);
    
private:
    Ui::playerWidget *ui;
    Phonon::MediaObject *mObject;
    Phonon::AudioOutput *aOutput;
    Phonon::Path path;

    PlaylistItem *selectedItem;

private slots:
    void aboutToFinishSlot();

    void on_toolButton_clicked();

    void on_nextBtn_clicked();

    void on_prevBtn_clicked();

signals:
    void aboutToFinish();
    void playNext();
    void playPrev();
};

#endif // PLAYERWIDGET_H
