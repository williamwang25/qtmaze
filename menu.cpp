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
        
        // 连接关闭信号，当mazeWidget关闭时重新显示菜单
        connect(gameWidget, &mazeWidget::destroyed, [this]() {
            gameWidget = nullptr;
            this->show();
        });
    }
    
    gameWidget->show();
    this->hide();
}


void menu::on_race_btn_clicked()
{
    if (!gameWidget) {
        gameWidget = new mazeWidget();
        
        connect(gameWidget, &mazeWidget::destroyed, [this]() {
            gameWidget = nullptr;
            this->show();
        });
    }
    gameWidget->show();
    this->hide();

}


void menu::on_rank_btn_clicked()
{

}


void menu::on_exit_btn_clicked()
{
    QApplication::quit();
}

