#include "menu.h"
#include "ui_menu.h"

menu::menu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::menu)
    , gameWidget(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Maze");
}

menu::~menu()
{
    delete ui;
}

void menu::on_start_btn_clicked()
{
    if (!gameWidget) {
        gameWidget = new mazeWidget();
        
        // 连接返回菜单的信号
        connect(gameWidget, &mazeWidget::backToMenu, this, [this]() {
            show(); // 显示菜单
        });
    }
    
    gameWidget->show();
    hide();
}


void menu::on_race_btn_clicked()
{
    if (!gameWidget) {
        gameWidget = new mazeWidget();
        
        // 连接返回菜单的信号
        connect(gameWidget, &mazeWidget::backToMenu, this, [this]() {
            show(); // 显示菜单
        });
    }
    
    // 显示竞赛按钮
    gameWidget->findChild<QPushButton*>("compete_button")->setVisible(true);
    
    gameWidget->show();
    hide();
}


void menu::on_rank_btn_clicked()
{
    rank* rankWidget = new rank();
    rankWidget->showRanking();
    rankWidget->show();
}

void menu::on_exit_btn_clicked()
{
    QApplication::quit();
}

