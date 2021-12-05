#include "bookitemwidget.hpp"

#include "ui_bookitemwidget.h"

BookItemWidget::BookItemWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::BookItemWidget)
{
    ui->setupUi(this);
}

BookItemWidget::~BookItemWidget() { delete ui; }
