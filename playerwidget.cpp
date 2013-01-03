#include "playerwidget.h"
#include "ui_playerwidget.h"

playerWidget::playerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::playerWidget)
{
    ui->setupUi(this);
}

playerWidget::~playerWidget()
{
    delete ui;
}

void playerWidget::play(PlaylistItem *item){

}

void playerWidget::fillFields(PlaylistItem *item){
    ui->songLbl->setText(tr("%1 - %2").arg(item->artist)
                         .arg(item->song));
    ui->timeLbl->setText(time(item->time.toInt()));
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
