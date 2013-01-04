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

    save = new QShortcut(this);
    save->setKey(QKeySequence(Qt::SHIFT + Qt::Key_Escape));
    connect(save, SIGNAL(activated()), this, SLOT(saveFile()));

    plAnimation = new QPropertyAnimation(this, "geometry");

    plModel = new PlaylistModel;
    albumsModel = new QStandardItemModel();

    cVk = new VK;
    cVk->connectToVk();

    connect(cVk, SIGNAL(albumsDone(QHash<QString,albums>*)), this, SLOT(albumsAdd(QHash<QString,albums>*)));
    connect(cVk, SIGNAL(audioDone()), this, SLOT(addAll()));
    connect(cPlayerWidget, SIGNAL(playNext()), this, SLOT(playNext()));
    connect(cPlayerWidget, SIGNAL(playPrev()), this, SLOT(playPrev()));
    connect(cPlayerWidget, SIGNAL(aboutToFinish()), this, SLOT(playNext()));

    progDialog = new QProgressDialog(this);
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
    play(index);
}

void Roster::play(const QModelIndex &index){
    currentIndex = index;
    PlaylistItem *item = plModel->data(index, Qt::DisplayRole).value<PlaylistItem*>();
    cPlayerWidget->play(item);
}

void Roster::on_plView_clicked(const QModelIndex &index)
{
    cPlayerWidget->saveSelected(plModel->data(index, Qt::DisplayRole).value<PlaylistItem*>());
}

void Roster::playNext(){
    QModelIndex index;
    if (currentIndex.isValid()) {
        index = plModel->index(currentIndex.row() + 1);
        if (index.isValid())
            play(index);
    }
}

void Roster::playPrev(){
    QModelIndex index;
    if (currentIndex.isValid()) {
        index = plModel->index(currentIndex.row() - 1);
        if (index.isValid())
            play(index);
    }
}

void Roster::saveFile(){
    QItemSelectionModel *selModel = ui->plView->selectionModel();
    QModelIndex index = selModel->selectedIndexes().at(0);
    PlaylistItem *item = plModel->data(index, Qt::DisplayRole).value<PlaylistItem*>();

    QString name = QFileDialog::getSaveFileName(this,
                                                tr("Сохранить песню"),
                                                QDir::homePath() + "/" + QString(item->artist + " - " + item->song),
                                                tr("Аудио (*.mp3)"));
    if (name.isEmpty())
        return;

    file.setFileName(name);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, qApp->applicationName(),
                                 tr("Ошибка сохранения файла %1: %2.")
                                 .arg(name).arg(file.errorString()));
        file.remove();
        return;
    }

    QNetworkAccessManager manager;
    QEventLoop loop;

    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(item->url)));
    connect( reply, SIGNAL(finished()), &loop, SLOT(quit()) );
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(progressDownSlot(qint64, qint64)));
    progDialog->show();
    progDialog->setLabelText(tr("Качаем..."));
    loop.exec();
    if (reply->error() == QNetworkReply::NoError){
        file.write(reply->readAll());
    }
    file.close();
}

void Roster::progressDownSlot(qint64 readBytes, qint64 totalBytes){
    progDialog->setMaximum(totalBytes);
    progDialog->setValue(readBytes);
    if (readBytes == totalBytes) {
        QMessageBox::information(this,
                                 qApp->applicationName(),
                                 tr("Песня скачана"),
                                 QMessageBox::Ok);
    }
}
