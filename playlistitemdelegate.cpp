#include "playlistitemdelegate.h"

PlaylistItemDelegate::PlaylistItemDelegate()
{
    wdg = new PlaylistItemWidget;
}

void PlaylistItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    /* Чтобы произвести данные преобразования нужно было регистрировать
     * тип макросом Q_DECLARE_METATYPE(MyItem *)
     */
    PlaylistItem *item = index.data().value<PlaylistItem *>();
    wdg->artist->setText(item->artist);
    wdg->song->setText("  " + item->song);
    wdg->time->setText(item->time);

    QPalette pal;
    QPalette textPal;
    QLinearGradient gradient(0, 0, 0, 100);

    /* Если элемент выделен то устанавливаем определенный цвет,
     * если нет, то устанавливаем градиент
     */
    if ((option.state & QStyle::State_Selected) == QStyle::State_Selected){
        pal.setBrush(QPalette::Window, QBrush(QColor(94, 130, 168)));
        textPal.setColor(QPalette::WindowText, Qt::white);
    }else{
//        gradient.setColorAt(0.0, QColor(255,250,0));
//        gradient.setColorAt(1.0, QColor(255,255,255));
//        pal.setBrush(QPalette::Window, QBrush(QColor(Qt::transparent)));
//        pal.setBrush(QPalette::Window, QBrush(gradient));
        pal.setBrush(QPalette::Window, QBrush(QColor(Qt::transparent)));
        textPal.setColor(QPalette::WindowText, QColor(43, 88, 122));
    }
    wdg->setPalette(pal);
    wdg->artist->setPalette(textPal);
    wdg->song->setPalette(textPal);
    wdg->time->setPalette(textPal);
    wdg->resize(option.rect.size().width(), 30);
    /* сохраняем текущее состояние painter'а, для того
     * чтобы произветси изменения над ним */
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->translate(option.rect.topLeft());
    wdg->render(painter);
    /*востанавливаем состояние пеинтера*/
    painter->restore();
}

QSize PlaylistItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    /*указываем любой нужный для нас размер*/
    int width = 80;
    int height = 36;
    return QSize(width, height);
}
