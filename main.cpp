#include "menu.h" //菜单类头文件
#include "mazewidget.h"     //迷宫窗口类头文件
#include "maze.h"           //迷宫类头文件
#include <QApplication>
#include <QStyleFactory>    //界面风格头文件

int main(int argc, char* argv[]) {
    srand(time(NULL));                              //设置随机数种子
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));    //更改界面风格
    menu w;
    w.show();
    return a.exec();
}
