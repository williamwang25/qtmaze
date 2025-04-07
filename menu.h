#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include "mazewidget.h"
#include "rank.h"

namespace Ui {
class menu;
}

class menu : public QWidget
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = nullptr);
    ~menu();

private slots:
    void on_start_btn_clicked();

    void on_race_btn_clicked();

    void on_rank_btn_clicked();

    void on_exit_btn_clicked();

private:
    Ui::menu *ui;
    mazeWidget *gameWidget;
};

#endif // MENU_H
