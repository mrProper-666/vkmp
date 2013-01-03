#include "roster.h"
#include "ui_roster.h"

Roster::Roster(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Roster)
{
    ui->setupUi(this);

    createInterface();

    ePlStatus = opened;
    plShortcutUp = new QShortcut(this);
    plShortcutUp->setKey(QKeySequence(Qt::CTRL + Qt::Key_Up));
    connect(plShortcutUp, SIGNAL(activated()), this, SLOT(plActivityUp()));

    plShortcutDown = new QShortcut(this);
    plShortcutDown->setKey(QKeySequence(Qt::CTRL + Qt::Key_Down));
    connect(plShortcutDown, SIGNAL(activated()), this, SLOT(plActivityDown()));

    plAnimation = new QPropertyAnimation(this, "geometry");

    plModel = new PlaylistModel;
    albumsModel = new QStandardItemModel();

    cVk = new VK;
    cVk->connectToVk();

    connect(cVk, SIGNAL(albumsDone(QHash<QString,albums>*)), this, SLOT(albumsAdd(QHash<QString,albums>*)));
    connect(cVk, SIGNAL(audioDone()), this, SLOT(addAll()));
}

Roster::~Roster()
{
    delete ui;
}

void Roster::createInterface(){

    this->setWindowTitle(qApp->applicationName() + " " + qApp->applicationVersion());
    cPlayerWidget = new playerWidget;
    ui->verticalLayout->insertWidget(0,cPlayerWidget);
}

void Roster::plActivityUp(){
    if (ePlStatus == opened) {
        QRect geom = this->geometry();
        widgRect = geom;
        plAnimation->setStartValue(geom);
        geom.setHeight(cPlayerWidget->height());
        plAnimation->setEndValue(geom);
        plAnimation->setEasingCurve(QEasingCurve::OutCubic);
        plAnimation->start();
        ePlStatus = closed;
        ui->plView->setVisible(false);
        ui->albView->setVisible(false);
    }
}

void Roster::plActivityDown(){
    if (ePlStatus == closed) {
        QRect geom = this->geometry();
        plAnimation->setStartValue(geom);
        plAnimation->setEndValue(widgRect);
        plAnimation->setEasingCurve(QEasingCurve::OutCubic);
        plAnimation->start();
        ePlStatus = opened;
        ui->plView->setVisible(true);
        ui->albView->setVisible(true);
    }
}

void Roster::createPl(QString id){
    plModel->clear();
    QList<PlaylistItem *> lst;

//    PlaylistItem *item1 = new PlaylistItem;
//    item1->artist = tr("Артист 1");
//    item1->song = tr("Песня 1");
//    item1->time = tr("3:43");
//    PlaylistItem *item2 = new PlaylistItem;
//    item2->artist = tr("Артист 2");
//    item2->song = tr("Песня 2");
//    item2->time = tr("13:03");
//    PlaylistItem *item3 = new PlaylistItem;
//    item3->artist = tr("Артист 3");
//    item3->song = tr("Песня 3");
//    item3->time = tr("2:16");
//    PlaylistItem *item4 = new PlaylistItem;
//    item4->artist = tr("Артист 4");
//    item4->song = tr("Песня 4");
//    item4->time = tr("4:21");

//    lst.append(item1);
//    lst.append(item2);
//    lst.append(item3);
//    lst.append(item4);
    QList<audio> audioList = cVk->albumById(id);

    foreach (audio s_audio, audioList) {
        PlaylistItem *item = new PlaylistItem;
        item->artist = s_audio.artist;
        item->song = s_audio.title;
        item->time = cPlayerWidget->time(s_audio.duration);
        item->id = s_audio.aid;
        item->aid = s_audio.album_id;
        item->url = s_audio.url;
        lst.prepend(item);
    }

    cPlayerWidget->fillFields(lst.at(0));
    plModel->list = lst; // устанавливаем список элементов
    ui->plView->setModel(plModel); // устанавливаем модель
    ui->plView->setItemDelegate(new PlaylistItemDelegate); // устанавливаем делегат
    ui->plView->repaint();
    ui->plView->scrollToTop();
}

void Roster::albumsAdd(QHash<QString,albums>* albumsHash){
//    QStringList strList;
//    strList << tr("Все");
//    foreach (QString key, albumsHash->keys()){
//        strList << albumsHash->value(key).title;
//    }
//    albumsModel = new QStringListModel();
//    albumsModel->setStringList(strList);
//    ui->albView->setModel(albumsModel);

    albumsModel->clear();
    QStandardItem *item = new QStandardItem();
    item->setText(tr("Все"));
    item->setAccessibleText("0");
    albumsModel->appendRow(item);

    foreach (QString key, albumsHash->keys()){
        QStandardItem *item = new QStandardItem();
        item->setText(albumsHash->value(key).title);
        item->setAccessibleText(albumsHash->value(key).id);
        albumsModel->appendRow(item);
    }
    ui->albView->setModel(albumsModel);
}

void Roster::addAll(){
    createPl("0");
}

void Roster::on_albView_clicked(const QModelIndex &index)
{
    createPl(albumsModel->itemFromIndex(index)->accessibleText());
}


void Roster::on_plView_doubleClicked(const QModelIndex &index)
{
    PlaylistItem *item = plModel->data(index, Qt::DisplayRole).value<PlaylistItem*>();
    qDebug() << item->url;
}
