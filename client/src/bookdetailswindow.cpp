#include "bookdetailswindow.h"

#include "ui_bookdetailswindow.h"

detailsbookwindow::detailsbookwindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::detailsbookwindow)
{
    ui->setupUi(this);
}

detailsbookwindow::~detailsbookwindow() { delete ui; }
