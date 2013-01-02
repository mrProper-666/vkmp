#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>

namespace Ui {
class playerWidget;
}

class playerWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit playerWidget(QWidget *parent = 0);
    ~playerWidget();
    
private:
    Ui::playerWidget *ui;
};

#endif // PLAYERWIDGET_H
