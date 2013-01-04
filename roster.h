#ifndef ROSTER_H
#define ROSTER_H

#include <QMainWindow>
#include <QLayout>
#include <QShortcut>
#include <QPropertyAnimation>
#include <QtDebug>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QFile>
#include <QtGui/QMessageBox>
#include <QNetworkAccessManager>
#include <QProgressDialog>

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
    QShortcut *save;
    QPropertyAnimation *plAnimation;
    QRect widgRect;
    PlaylistModel *plModel;
    VK *cVk;
    QStandardItemModel *albumsModel;
    QModelIndex currentIndex;
    QFile file;
    QProgressDialog *progDialog;
    QEventLoop loop;

    void createInterface();
    void createPl(QString id);
    QString time(int time);
    void play(const QModelIndex &index);

private slots:
    void plActivityUp();
    void plActivityDown();
    void albumsAdd(QHash<QString,albums>*);
    void addAll();
    void playNext();
    void playPrev();
    void saveFile();
    void progressDownSlot(qint64, qint64);
    void cancelDownload();

    void on_albView_clicked(const QModelIndex &index);
    void on_plView_doubleClicked(const QModelIndex &index);
    void on_plView_clicked(const QModelIndex &index);
};

#endif // ROSTER_H
