#ifndef TREEWINDOW_H
#define TREEWINDOW_H

#include <QWidget>

namespace Ui {
class treewindow;
}

class treewindow : public QWidget
{
    Q_OBJECT

public:
    explicit treewindow(QWidget *parent = nullptr);
    ~treewindow();

private:
    Ui::treewindow *ui;
};

#endif // TREEWINDOW_H
