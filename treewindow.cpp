#include "treewindow.h"
#include "ui_treewindow.h"

treewindow::treewindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::treewindow)
{
    ui->setupUi(this);
}

treewindow::~treewindow()
{
    delete ui;
}
