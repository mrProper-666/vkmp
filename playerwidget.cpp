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
