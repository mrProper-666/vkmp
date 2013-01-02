#ifndef ROSTER_H
#define ROSTER_H

#include <QMainWindow>
#include <QLayout>
#include <QShortcut>
#include <QPropertyAnimation>
#include <QtDebug>
#include <QStringListModel>

#include "playerwidget.h"
#include "playlistmodel.h"
#include "playlistitem.h"
#include "playlistitemdelegate.h"
#include "vk.h"

typedef enum {
    opened,
    closed
} plStatus;

namespace Ui {
class Roster;
}

class Roster : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Roster(QWidget *parent = 0);
    ~Roster();
    
private:
    Ui::Roster *ui;
    playerWidget *cPlayerWidget;
    QVBoxLayout *mainLayout;
    plStatus ePlStatus;
    QShortcut *plShortcutUp;
    QShortcut *plShortcutDown;
    QPropertyAnimation *plAnimation;
    QRect widgRect;
    PlaylistModel *plModel;
    VK *cVk;
    QStringListModel *albumsModel;

    void createInterface();
    void createPl();

private slots:
    void plActivityUp();
    void plActivityDown();
    void albumsAdd(QHash<QString,albums>*);

};

#endif // ROSTER_H
