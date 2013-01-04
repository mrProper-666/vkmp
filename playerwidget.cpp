#include "playerwidget.h"
#include "ui_playerwidget.h"

playerWidget::playerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::playerWidget)
{
    ui->setupUi(this);
    mObject = new Phonon::MediaObject(this);
    aOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    path = Phonon::createPath(mObject, aOutput);
    ui->volumeSlider->setAudioOutput(aOutput);
    ui->seekSlider->setMediaObject(mObject);

    connect(mObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinishSlot()));
}

playerWidget::~playerWidget()
{
    delete ui;
}

void playerWidget::play(PlaylistItem *item){
    ui->songLbl->setText(tr("%1 - %2").arg(item->artist)
                         .arg(item->song));
    ui->timeLbl->setText(item->time);
    mObject->setCurrentSource(item->url);
    mObject->play();
}

void playerWidget::fillFields(PlaylistItem *item){
    ui->songLbl->setText(tr("%1 - %2").arg(item->artist)
                         .arg(item->song));
    ui->timeLbl->setText(item->time);
}

QString playerWidget::time(int time){
    QString timeString;
    int sec = time;
    int min = sec / 60;
    int hour = min / 60;
    int msec = time;

    QTime playTime(hour%60, min%60, sec%60, msec%1000);
    QString timeFormat = "m:ss";
    if (hour > 0)
        timeFormat = "h:mm:ss";
    timeString = playTime.toString(timeFormat);
    return timeString;
}

void playerWidget::aboutToFinishSlot(){
    emit aboutToFinish();
}

void playerWidget::saveSelected(PlaylistItem *item){
    selectedItem = item;
}

void playerWidget::on_toolButton_clicked()
{
    if (selectedItem)
        play(selectedItem);
}

void playerWidget::on_nextBtn_clicked()
{
    emit playNext();
}

void playerWidget::on_prevBtn_clicked()
{
    emit playPrev();
}
