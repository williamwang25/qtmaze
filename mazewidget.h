#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H


#include <QWidget>
#include <QMessageBox>      //Qt信息窗口头文件
#include <QPainter>         //Qt绘图头文件
#include <QDebug>           //QtDebug头文件
#include <QKeyEvent>        //Qt按键事件头文件
#include <QTimer>           //Qt计时器头文件
#include <QInputDialog>     //Qt输入对话框头文件
#include "maze.h"           //迷宫类头文件

QT_BEGIN_NAMESPACE
namespace Ui {
class mazeWidget;
}
QT_END_NAMESPACE
class mazeWidget : public QWidget {
    Q_OBJECT
public:
    mazeWidget(QWidget* parent = nullptr);
    ~mazeWidget();

protected:
    void paintEvent(QPaintEvent*);        //绘图事件
    void keyPressEvent(QKeyEvent*);       //按键按下事件
signals:
    void backToMenu(); // 返回菜单
private slots:
    void on_start_btn_clicked();    //|
    void on_stop_ptn_clicked();     //|
    void on_end_btn_clicked();      //|
    void on_rule_btn_clicked();     //|各按钮点击槽函数
    void on_setting_btn_clicked();  //|
    void time_update();             //时间更新槽函数
    void on_solve_btn_clicked();
    void on_searchOver();
    void maze_repaint();
    
    void on_compete_button_clicked(); // 竞赛模式按钮
    void competitionStepUpdate(); // 定时更新竞赛状态
    void onCompetitionOver(int winner); // 处理竞赛结束


    void on_dfs_btn_clicked();

    void on_bfs_btn_clicked();

    void on_menu_btn_clicked();

private:
    Ui::mazeWidget* ui;         //ui对象
    maze* map;//迷宫对象

    bool painting_switch;       //绘图开关
    bool timing_switch;         //计时开关
    bool keybord_switch;        //键盘响应开关
    bool stop_switch;           //暂停按钮状态

    int grade;                  //分数
    int time;                   //时间
    QTimer* timer;
    QTimer* competeTimer;       // 竞赛定时器
    
    bool competeMode; // 是否为竞赛模式
    bool competitionStepUpdate_resetFirstCheck();
    bool isWalkable(int value); // 判断格子是否可行走


};
#endif // MAZEWIDGET_H
