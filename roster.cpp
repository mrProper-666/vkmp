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
    createPl();
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

void Roster::createPl(){
    QList<PlaylistItem *> lst;

    PlaylistItem *item1 = new PlaylistItem;
    item1->artist = tr("Артист 1");
    item1->song = tr("Песня 1");
    item1->time = tr("3:43");
    PlaylistItem *item2 = new PlaylistItem;
    item2->artist = tr("Артист 2");
    item2->song = tr("Песня 2");
    item2->time = tr("13:03");
    PlaylistItem *item3 = new PlaylistItem;
    item3->artist = tr("Артист 3");
    item3->song = tr("Песня 3");
    item3->time = tr("2:16");
    PlaylistItem *item4 = new PlaylistItem;
    item4->artist = tr("Артист 4");
    item4->song = tr("Песня 4");
    item4->time = tr("4:21");

    lst.append(item1);
    lst.append(item2);
    lst.append(item3);
    lst.append(item4);

    plModel->list = lst; // устанавливаем список элементов
    ui->plView->setModel(plModel); // устанавливаем модель
    ui->plView->setItemDelegate(new PlaylistItemDelegate); // устанавливаем делегат
}
